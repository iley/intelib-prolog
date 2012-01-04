#include "data.hpp"
#include "engine.hpp"
#include "utils.hpp"

static SListConstructor S;

// Generic Prolog Expression

PlgReference PlgUnbound;
PlgTruthValue PlgTrue;

bool PlgObject::Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const {
    return self.GetPtr() == other.GetPtr();
}

// STUB
PlgReference PlgObject::RenameVars(const PlgReference &self, PlgContext &context, SHashTable &existingVars) const {
    return self;
}

// Reference to a generic prolog expression

bool PlgReference::Unify(const PlgReference &other, PlgContext &context) const {
    int pos = context.Top();

    //FIXME: unnecessary evaluation
    PlgReference left = context.Evaluate(*this),
        right = context.Evaluate(other);

    if (
        left->TermType() != PlgExpressionVariableIndex::TypeId
        && right->TermType() == PlgExpressionVariableIndex::TypeId
    ) {
        PlgReference tmp = left;
        left = right;
        right = tmp;
    }

    const PlgObject *obj = dynamic_cast<const PlgObject*>(left.GetPtr());
    INTELIB_ASSERT(obj, IntelibX_not_a_prolog_object(*this));
    bool result = obj->Unify(left, right, context);

    if (!result)
        context.ReturnTo(pos, false);
    return result;
}

// Truth value

IntelibTypeId PlgExpressionTruthValue::TypeId(&SExpression::TypeId, false);

bool PlgExpressionTruthValue::Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const {
    return other->TermType() == PlgExpressionTruthValue::TypeId;
}

// Clause

IntelibTypeId PlgExpressionClause::TypeId(&SExpression::TypeId, true);

PlgReference PlgExpressionClause::RenameVars(const PlgReference &self, PlgContext &context, SHashTable &existingVars) const {
    return PlgClause(head.RenameVars(context, existingVars), body.RenameVars(context, existingVars));
}

PlgClause operator <<= (const PlgReference &head, const PlgReference &body) {
    INTELIB_ASSERT(head->TermType() == PlgExpressionTerm::TypeId, IntelibX_not_a_prolog_term(head));
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

bool PlgExpressionUserPredicate::Apply(const SReference &args, PlgExpressionContinuation &cont) {
    return function(args, cont);
}

// Atom

IntelibTypeId PlgExpressionAtom::TypeId(&SExpression::TypeId, true);

PlgPredicate PlgExpressionAtom::GetPredicate(int arity) const {
    INTELIB_ASSERT(arity >= 0, IntelibX_invalid_arity(arity));
    PlgPredicate result;

    if (arity < predicates->Size())
        result = predicates[arity];

    if (!result.GetPtr())
        result = varArgPredicate;

    // can return undefined value, caller should check
    return result;
}

PlgReference PlgAtom::operator() (const PlgReference &arg1) {
    return PlgTerm(*this, (S| arg1 ));
}

PlgReference PlgAtom::operator() (const PlgReference &arg1, const PlgReference &arg2) {
    return PlgTerm(*this, (S| arg1, arg2 ));
}

PlgReference PlgAtom::operator() (const PlgReference &arg1, const PlgReference &arg2, const PlgReference &arg3) {
    return PlgTerm(*this, (S| arg1, arg2, arg3 ));
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

SString PlgExpressionVariableIndex::TextRepresentation() const {
    static char buffer[32]; //FIXME
    sprintf(buffer, "<VAR #%d>", value);
    return buffer;
}