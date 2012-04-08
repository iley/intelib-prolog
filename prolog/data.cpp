#include "data.hpp"
#include "engine.hpp"
#include "utils.hpp"
#include "library/library.hpp"
#include "../sexpress/shashtbl.hpp"

#include <string.h>

const char *NewVarName()
{
    static char buffer[32]; // 32 ought to be enough for anybody!
    static int count = 0;
    sprintf(buffer, "_G%d", count++);
    return buffer;
}

static SListConstructor S;

// Generic Prolog Expression

PlgHooks PlgGlobalHooks;
PlgReference PlgUnbound;

bool PlgObject::Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const
{
    return self.IsEql(other);
}

PlgReference PlgObject::RenameVars(const PlgReference &self, PlgContext &context, SHashTable &existingVars) const
{
    return self;
}

PlgReference PlgObject::Evaluate(const PlgReference &self, PlgContext &context) const
{
    return self;
}

// Reference to a generic prolog expression

bool PlgReference::Unify(const PlgReference &other, PlgContext &context) const
{
    if (PlgGlobalHooks.UnifyCall)
        PlgGlobalHooks.UnifyCall(*this, other, context);

    int pos = context.NextFrame();
    bool result;

    /* unnecessary evaluation? */
    PlgReference left = Evaluate(context),
        right = other.Evaluate(context);
    /*
    PlgReference left = *this,
        right = other;*/

    if ((
            left->TermType() != PlgExpressionVariable::TypeId
            && right->TermType() == PlgExpressionVariable::TypeId
        ) || (
            left->TermType() != PlgExpressionAnonymousVariable::TypeId
            && right->TermType() == PlgExpressionAnonymousVariable::TypeId
        )
    ) {
        PlgReference tmp = left;
        left = right;
        right = tmp;
    }

    // 0-arity terms are atoms actually
    if (left->TermType() == PlgExpressionTerm::TypeId && PlgTerm(left)->Arity() == 0)
        left = PlgTerm(left)->Functor();

    if (right->TermType() == PlgExpressionTerm::TypeId && PlgTerm(right)->Arity() == 0)
        right = PlgTerm(right)->Functor();

    const PlgObject *obj = dynamic_cast<const PlgObject*>(left.GetPtr());
    if (obj) {
        result = obj->Unify(left, right, context);
    } else if (left->TermType() == SExpressionCons::TypeId) {
        result = right->TermType() == SExpressionCons::TypeId
            && PlgReference(left.Car()).Unify(right.Car(), context)
            && PlgReference(left.Cdr()).Unify(right.Cdr(), context);
    } else {
        result = left.IsEqual(right);
    }


    if (!result)
        context.ReturnTo(pos, false);

    if (PlgGlobalHooks.UnifyExit)
        PlgGlobalHooks.UnifyExit(*this, other, context, result);

    return result;
}

PlgReference PlgReference::RenameVars(PlgContext &context, SHashTable &existingVars) const
{
    INTELIB_ASSERT(GetPtr(), IntelibX_unexpected_unbound_value());

    PlgReference result;
    PlgObject *obj = dynamic_cast<PlgObject*>(GetPtr());
    if (obj) {
        result =  obj->RenameVars(*this, context, existingVars);
    } else if ((*this)->TermType() == SExpressionCons::TypeId) {
        SReference newCar = PlgReference(Car()).RenameVars(context, existingVars),
                   newCdr = PlgReference(Cdr()).RenameVars(context, existingVars);
        if (newCar == Car() && newCdr == Cdr())
            result = *this;
        else
            result = newCar ^ newCdr;
    } else {
        result =  *this;
    }

    if (PlgGlobalHooks.Rename)
        PlgGlobalHooks.Rename(*this, result, context);
    return result;
}

PlgReference PlgReference::Evaluate(PlgContext &context) const
{
    if (PlgGlobalHooks.Evaluate)
        PlgGlobalHooks.Evaluate(*this, context);

    INTELIB_ASSERT(GetPtr(), IntelibX_unexpected_unbound_value());

    PlgObject *obj = dynamic_cast<PlgObject*>(GetPtr());
    if (obj) {
        return obj->Evaluate(*this, context);
    } else if ((*this)->TermType() == SExpressionCons::TypeId) {
        SReference newCar = PlgReference(Car()).Evaluate(context),
                   newCdr = PlgReference(Cdr()).Evaluate(context);

        if (newCar == Car() && newCdr == Cdr())
            return *this;
        else
            return newCar ^ newCdr;
    } else {
        return *this;
    }
}

PlgReference PlgReference::Functor() const
{
    if ((*this)->TermType() == PlgExpressionAtom::TypeId)
        return *this;
    else if ((*this)->TermType() == PlgExpressionTerm::TypeId)
        return SimpleCastGetPtr<PlgExpressionTerm>()->Functor();
    else
        throw IntelibX_bug();
}

SReference PlgReference::Args() const
{
    if ((*this)->TermType() == PlgExpressionAtom::TypeId)
        return *PTheEmptyList;
    else if ((*this)->TermType() == PlgExpressionTerm::TypeId)
        return SimpleCastGetPtr<PlgExpressionTerm>()->Args();
    else
        throw IntelibX_bug();
}

PlgReference PlgReference::Head() const
{
    if ((*this)->TermType() == PlgExpressionTerm::TypeId) {
        PlgTerm term = *this;
        if (term->Functor().IsEql(PlgStdLib::implication))
            return term->Args().Car();
        else
            return term;
    } else {
        return *this;
    }
}

PlgReference PlgReference::Body() const
{
    if ((*this)->TermType() == PlgExpressionTerm::TypeId) {
        PlgTerm term = *this;
        if (term->Functor().IsEql(PlgStdLib::implication))
            return term->Args().Cdr().Car();
        else
            return PlgStdLib::truth;
    } else {
        return PlgStdLib::truth;
    }
}

// Term

IntelibTypeId PlgExpressionTerm::TypeId(&SExpression::TypeId, false);

PlgExpressionTerm::PlgExpressionTerm(const PlgAtom &fn, const SReference &as) : SExpression(TypeId), functor(fn), args(as)
{}

PlgExpressionTerm::PlgExpressionTerm(const IntelibTypeId &typeId, const PlgAtom &fn, const SReference &as) : SExpression(typeId), functor(fn), args(as)
{}

int PlgExpressionTerm::Arity() const
{ //TODO: cache arity value
    return Length(args);
}

bool PlgExpressionTerm::Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const
{
    if (other->TermType() != PlgExpressionTerm::TypeId)
        return false;

    PlgExpressionTerm *otherTerm = other.SimpleCastGetPtr<PlgExpressionTerm>();
    if (functor != otherTerm->functor)
        return false;

    SReference ourArgs = args, theirArgs = otherTerm->args;
    while (!ourArgs.IsEmptyList() && !theirArgs.IsEmptyList()) {
        PlgReference ourArg = ourArgs.Car();
        PlgReference theirArg = theirArgs.Car();

        if (!ourArg.Unify(theirArg, context)) {
            return false;
        }

        ourArgs = ourArgs.Cdr();
        theirArgs = theirArgs.Cdr();
    }

    return ourArgs.IsEmptyList() && theirArgs.IsEmptyList();
}

PlgReference PlgExpressionTerm::RenameVars(const PlgReference &self, PlgContext &context, SHashTable &existingVars) const
{
    return PlgTerm(functor, PlgReference(Args()).RenameVars(context, existingVars));
}


PlgReference PlgExpressionTerm::Evaluate(const PlgReference &self, PlgContext &context) const
{
    return PlgTerm(Functor(), PlgReference(Args()).Evaluate(context));
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionTerm::TextRepresentation() const
{
    if (functor->IsInfix()) {
        return Join(functor->TextRepresentation(), args);
    } else {
        if (args.IsEmptyList())
            return functor->TextRepresentation();
        else
            return functor->TextRepresentation() + "(" + Join(", ", args) + ")";
    }
}
#endif

// Predicate

IntelibTypeId PlgExpressionPredicate::TypeId(&SExpression::TypeId, false);

bool PlgExpressionUserPredicate::Apply(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
{
    if (PlgGlobalHooks.Call)
        PlgGlobalHooks.Call(functor, args, cont);
    bool result = function(functor, args, cont);
    if (PlgGlobalHooks.Exit)
        PlgGlobalHooks.Exit(functor, args, cont, result);
    return result;
}

// Procedure table

IntelibTypeId PlgExpressionProcTable::TypeId(&SExpression::TypeId, true);

PlgPredicate PlgExpressionProcTable::GetPredicate(int arity) const
{
    INTELIB_ASSERT(arity >= 0, IntelibX_invalid_arity(arity));

    if (arity < predicates->Size() && predicates[arity].GetPtr())
        return predicates[arity];
    else if (varArgPredicate.GetPtr())
        return varArgPredicate;
    else
        return PlgDefaultPredicate;
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionProcTable::TextRepresentation() const
{
    return "<PROCEDURE TABLE>";
}
#endif

static SHashTable &GlobalAtomTable() {
    static SHashTable table;
    return table;
}

// Atom

IntelibTypeId PlgExpressionAtom::TypeId(&SExpression::TypeId, true);

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionAtom::TextRepresentation() const
{
    return GetValue();
}
#endif

PlgAtom::PlgAtom(const char *name, bool infix) : PlgAtom_Super(new PlgExpressionAtom(name, infix)) {}

PlgAtom::PlgAtom(const char *name, int arity, const PlgPredicate &pred, bool infix) : PlgAtom_Super(new PlgExpressionAtom(name, infix)) {
    GetProcTable()->SetPredicate(pred, arity);
}

PlgAtom::PlgAtom(const char *name, const PlgPredicate &pred, bool infix) : PlgAtom_Super(new PlgExpressionAtom(name, infix)){
    GetProcTable()->SetPredicate(pred);
}

PlgProcTable PlgAtom::GetProcTable() const
{
    PlgProcTable tbl = GlobalAtomTable()->FindItem(*this, PlgUnbound);
    if (!tbl.GetPtr()) {
        tbl = PlgProcTable(new PlgExpressionProcTable());
        GlobalAtomTable()->AddItem(*this, tbl);
    }
    return tbl;
}

PlgReference PlgAtom::operator() (const SReference &arg1)
{
    return PlgTerm(*this, (S| arg1 ));
}

PlgReference PlgAtom::operator() (const SReference &arg1, const SReference &arg2)
{
    return PlgTerm(*this, (S| arg1, arg2 ));
}

PlgReference PlgAtom::operator() (const SReference &arg1, const SReference &arg2, const SReference &arg3)
{
    return PlgTerm(*this, (S| arg1, arg2, arg3 ));
}

PlgReference PlgAtom::operator() (const SReference &arg1, const SReference &arg2, const SReference &arg3, const SReference &arg4)
{
    return PlgTerm(*this, (S| arg1, arg2, arg3, arg4 ));
}

PlgReference PlgAtom::operator() (const SReference &arg1, const SReference &arg2, const SReference &arg3, const SReference &arg4, const SReference &arg5)
{
    return PlgTerm(*this, (S| arg1, arg2, arg3, arg4, arg5 ));
}

PlgReference PlgAtom::operator() (const SReference &arg1, const SReference &arg2, const SReference &arg3, const SReference &arg4, const SReference &arg5, const SReference &arg6)
{
    return PlgTerm(*this, (S| arg1, arg2, arg3, arg4, arg5, arg6 ));
}

PlgReference PlgAtom::operator() (const SReference &arg1, const SReference &arg2, const SReference &arg3, const SReference &arg4, const SReference &arg5, const SReference &arg6, const SReference &arg7)
{
    return PlgTerm(*this, (S| arg1, arg2, arg3, arg4, arg5, arg6, arg7 ));
}
// TODO more args

// Variable Name

IntelibTypeId PlgExpressionVariable::TypeId(&SExpressionLabel::TypeId, false);

PlgReference PlgExpressionVariable::RenameVars(const PlgReference &self, PlgContext &context, SHashTable &existingVars) const
{
    PlgReference binding = existingVars->FindItem(self, PlgUnbound);
    if (binding.GetPtr()) {
        return binding;
    } else {
        PlgVariable newName(NewVarName());
        existingVars->AddItem(self, newName);
        return newName;
    }
}

bool PlgExpressionVariable::Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const
{
    context.Set(self, other);
    return true;
}

PlgReference PlgExpressionVariable::Evaluate(const PlgReference &self, PlgContext &context) const
{
    PlgReference binding = context.Get(self);
    if (!binding.GetPtr())
        return self;
    else
        return binding.Evaluate(context);
}

PlgVariable::PlgVariable(const char *name) : PlgVariable_Super(new PlgExpressionVariable(name)) {}

PlgReference PlgVariable::is(const PlgReference &expr)
{
    return PlgStdLib::is(*this, expr);
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionVariable::TextRepresentation() const
{
    return GetValue();
}
#endif

IntelibTypeId PlgExpressionAnonymousVariable::TypeId(&SExpression::TypeId, false);

bool PlgExpressionAnonymousVariable::Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const
{
    return true;
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionAnonymousVariable::TextRepresentation() const
{
    return "_";
}
#endif
