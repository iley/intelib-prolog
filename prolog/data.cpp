#include "data.hpp"
#include "engine.hpp"
#include "utils.hpp"
#include "library.hpp"

static SListConstructor S;

// Generic Prolog Expression

PlgHooks PlgGlobalHooks;
PlgReference PlgUnbound;

bool PlgObject::Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const {
    return self.GetPtr() == other.GetPtr();
}

PlgReference PlgObject::RenameVars(const PlgReference &self, PlgContext &context, SHashTable &existingVars) const {
    return self;
}

PlgReference PlgObject::Evaluate(const PlgReference &self, PlgContext &context) const {
    return self;
}

// Reference to a generic prolog expression

bool PlgReference::Unify(const PlgReference &other, PlgContext &context) const {
    if (PlgGlobalHooks.Unify)
        PlgGlobalHooks.Unify(*this, other, context);

    int pos = context.Top();
    bool result;

    /* unnecessary evaluation?
    PlgReference left = Evaluate(context),
        right = other.Evaluate(context); */
    PlgReference left = *this,
        right = other;

    if (
        left->TermType() != PlgExpressionVariableIndex::TypeId
        && right->TermType() == PlgExpressionVariableIndex::TypeId
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
    if (obj)
        result = obj->Unify(left, right, context);
    else if (left->TermType() == SExpressionCons::TypeId)
        result = right->TermType() == SExpressionCons::TypeId
            && PlgReference(left.Car()).Unify(right.Car(), context)
            && PlgReference(left.Cdr()).Unify(right.Cdr(), context);
    else
        result = left.IsEqual(right);
    

    if (!result)
        context.ReturnTo(pos, false);
    return result;
}

PlgReference PlgReference::RenameVars(PlgContext &context, SHashTable &existingVars) const {
    PlgObject *obj = dynamic_cast<PlgObject*>(GetPtr());
    if (obj)
        return obj->RenameVars(*this, context, existingVars);
    else if ((*this)->TermType() == SExpressionCons::TypeId)
        return SReference(
            PlgReference(Car()).RenameVars(context, existingVars),
            PlgReference(Cdr()).RenameVars(context, existingVars));
    else
        return *this;
}

PlgReference PlgReference::Evaluate(PlgContext &context) const {
    PlgObject *obj = dynamic_cast<PlgObject*>(GetPtr());
    if (obj)
        return obj->Evaluate(*this, context);
    else if ((*this)->TermType() == SExpressionCons::TypeId)
         return SReference(
            PlgReference(Car()).Evaluate(context),
            PlgReference(Cdr()).Evaluate(context));
    else
        return *this;
}

// Clause

IntelibTypeId PlgExpressionClause::TypeId(&SExpression::TypeId, true);

PlgReference PlgExpressionClause::RenameVars(const PlgReference &self, PlgContext &context, SHashTable &existingVars) const {
    return PlgClause(head.RenameVars(context, existingVars), body.RenameVars(context, existingVars));
}

PlgReference PlgExpressionClause::Evaluate(const PlgReference &self, PlgContext &context) const {
    return PlgClause(head.Evaluate(context), body.Evaluate(context));
}

PlgClause operator <<= (const PlgReference &head, const PlgReference &body) {
    PlgReference term;
    if (head->TermType() == PlgExpressionTerm::TypeId)
        term = head;
    else if (head->TermType() == PlgExpressionAtom::TypeId)
        term = PlgTerm(head, *PTheEmptyList);
    else
        throw IntelibX_bug();
    return PlgClause(head, body);
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionClause::TextRepresentation() const {
    return head->TextRepresentation() + " :- " + body->TextRepresentation() + ".";
}
#endif

// Term

IntelibTypeId PlgExpressionTerm::TypeId(&SExpression::TypeId, false);

PlgExpressionTerm::PlgExpressionTerm(const PlgAtom &fn, const SReference &as) : SExpression(TypeId), functor(fn), args(as) {}
PlgExpressionTerm::PlgExpressionTerm(const IntelibTypeId &typeId, const PlgAtom &fn, const SReference &as) : SExpression(typeId), functor(fn), args(as) {}

int PlgExpressionTerm::Arity() const { return Length(args); } //TODO: cache arity value

bool PlgExpressionTerm::Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const {
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

PlgReference PlgExpressionTerm::RenameVars(const PlgReference &self, PlgContext &context, SHashTable &existingVars) const {
    SReference newArgs = *PTheEmptyList;
    for (SReference p = args; !p.IsEmptyList(); p = p.Cdr()) {
        PlgReference pref = p.Car();
        newArgs.AddAnotherItemToList(pref.RenameVars(context, existingVars));
    }
    return PlgTerm(functor, newArgs);
}


PlgReference PlgExpressionTerm::Evaluate(const PlgReference &self, PlgContext &context) const {
    SReference resultArgs = *PTheEmptyList;
    PlgTerm term = self;

    for (SReference p = term->Args(); !p.IsEmptyList(); p = p.Cdr()) {
        resultArgs.AddAnotherItemToList(PlgReference(p.Car()).Evaluate(context));
    }

    return PlgTerm(term->Functor(), resultArgs);
}


#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionTerm::TextRepresentation() const {
    if (functor->IsInfix()) {
        return Join(functor->TextRepresentation(), args);
    } else {
        return functor->TextRepresentation() + "(" + Join(", ", args) + ")";
    }
}
#endif

// Predicate

IntelibTypeId PlgExpressionPredicate::TypeId(&SExpression::TypeId, false);

bool PlgExpressionUserPredicate::Apply(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
    if (PlgGlobalHooks.Call)
        PlgGlobalHooks.Call(functor, args, cont);
    return function(functor, args, cont);
}

// Atom

IntelibTypeId PlgExpressionAtom::TypeId(&SExpression::TypeId, true);

PlgPredicate PlgExpressionAtom::GetPredicate(int arity) const {
    INTELIB_ASSERT(arity >= 0, IntelibX_invalid_arity(arity));

    if (arity < predicates->Size() && predicates[arity].GetPtr())
        return predicates[arity];
    else if (varArgPredicate.GetPtr())
        return varArgPredicate;
    else
        return PlgDefaultPredicate;
}

PlgReference PlgAtom::operator() (const SReference &arg1) {
    return PlgTerm(*this, (S| arg1 ));
}

PlgReference PlgAtom::operator() (const SReference &arg1, const SReference &arg2) {
    return PlgTerm(*this, (S| arg1, arg2 ));
}

PlgReference PlgAtom::operator() (const SReference &arg1, const SReference &arg2, const SReference &arg3) {
    return PlgTerm(*this, (S| arg1, arg2, arg3 ));
}

PlgReference PlgAtom::operator() (const SReference &arg1, const SReference &arg2, const SReference &arg3, const SReference &arg4) {
    return PlgTerm(*this, (S| arg1, arg2, arg3, arg4 ));
}

PlgReference PlgAtom::operator() (const SReference &arg1, const SReference &arg2, const SReference &arg3, const SReference &arg4, const SReference &arg5) {
    return PlgTerm(*this, (S| arg1, arg2, arg3, arg4, arg5 ));
}
// TODO more args

// Variable Name

IntelibTypeId PlgExpressionVariableName::TypeId(&SExpressionLabel::TypeId, false);

PlgReference PlgExpressionVariableName::RenameVars(const PlgReference &self, PlgContext &context, SHashTable &existingVars) const {
    PlgReference binding = existingVars->FindItem(self, PlgUnbound);
    if (binding == PlgUnbound) {
        PlgVariableIndex idx(context.NextIndex());
        existingVars->AddItem(self, idx);
        return idx;
    } else {
        return binding;
    }
}

// Variable index

IntelibTypeId PlgExpressionVariableIndex::TypeId(&SExpression::TypeId, false);

PlgExpressionVariableIndex::PlgExpressionVariableIndex(PlgContext &ctx)
    : SExpression(TypeId), value(ctx.NextIndex()) {
}

bool PlgExpressionVariableIndex::Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const {
    PlgVariableIndex newVar(context);
    context.Set(newVar, other);
    context.Set(self, newVar);
    return true;
}

PlgReference PlgExpressionVariableIndex::Evaluate(const PlgReference &self, PlgContext &context) const {
    PlgReference val = self;

    while (val.GetPtr() && val->TermType() == PlgExpressionVariableIndex::TypeId) {
        val = context.Get(val);
    }

    if (val.GetPtr())
        return val.Evaluate(context);
    else
        return self;
}

SString PlgExpressionVariableIndex::TextRepresentation() const {
    static char buffer[32]; //FIXME
    sprintf(buffer, "_%d", value);
    return buffer;
}
