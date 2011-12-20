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
bool PlgObject::Solve(const PlgReference &self, PlgExpressionContinuation &continuation) const {
    throw IntelibX_not_implemented();
}

PlgReference PlgObject::RenameVars(const PlgReference &self, NameGeneratorFunction nameGenerator, SHashTable &existingVars) const {
    return self;
}

// Reference to a generic prolog expression

bool PlgReference::Unify(const PlgReference &other, PlgContext &context) const {
    PlgReference self = *this;
    context.CreateFrame();

    bool result;

    if (
        self->TermType() != PlgExpressionVariableName::TypeId
        && other->TermType() == PlgExpressionVariableName::TypeId
    ) {
        const PlgObject *otherObj = dynamic_cast<const PlgObject*>(other.GetPtr());
        INTELIB_ASSERT(otherObj, IntelibX_not_a_prolog_object(*this));
        result = otherObj->Unify(other, self, context);
    } else {
        const PlgObject *selfObj = dynamic_cast<const PlgObject*>(GetPtr());
        INTELIB_ASSERT(selfObj, IntelibX_not_a_prolog_object(*this));
        result = selfObj->Unify(self, other, context);
    }

    if (result)
        context.MergeDownFrame();
    else
        context.DropFrame();

    return result;
}

// Truth value

IntelibTypeId PlgExpressionTruthValue::TypeId(&SExpression::TypeId, false);

bool PlgExpressionTruthValue::Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const {
    return other->TermType() == PlgExpressionTruthValue::TypeId;
}

bool PlgExpressionTruthValue::Solve(const PlgReference &self, PlgExpressionContinuation &continuation) const {
    return true;
}

// Clause

IntelibTypeId PlgExpressionClause::TypeId(&SExpression::TypeId, true);

PlgReference PlgExpressionClause::RenameVars(const PlgReference &self, NameGeneratorFunction nameGenerator, SHashTable &existingVars) const {
    return PlgClause(head.RenameVars(nameGenerator, existingVars), body.RenameVars(nameGenerator, existingVars));
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

PlgExpressionTerm::PlgExpressionTerm(const PlgAtom &fn, const SReference &as) : SExpression(TypeId), functor(fn), args(as), arity(Length(as)) {}

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

bool PlgExpressionTerm::Solve(const PlgReference &self, PlgExpressionContinuation &continuation) const {
    PlgClauseChoicePoint cp(self, continuation.Database().Head(), continuation.Context().Top());
    continuation.PushChoicePoint(cp);
    return cp->Next(continuation);
}

PlgReference PlgExpressionTerm::RenameVars(const PlgReference &self, NameGeneratorFunction nameGenerator, SHashTable &existingVars) const {
    SReference newArgs = *PTheEmptyList;
    for (SReference p = args; !p.IsEmptyList(); p = p.Cdr()) {
        PlgReference pref = p.Car();
        newArgs.AddAnotherItemToList(pref.RenameVars(nameGenerator, existingVars));
    }
    return PlgTerm(functor, newArgs);
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionTerm::TextRepresentation() const { 
    return functor->TextRepresentation() + "(" + Join(", ", args) + ")";
}
#endif

// Atom

IntelibTypeId PlgExpressionAtom::TypeId(&SExpression::TypeId, false);

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

IntelibTypeId PlgExpressionVariableName::TypeId(&PlgExpressionAtom::TypeId, false);

bool PlgExpressionVariableName::Unify(const PlgReference &self, const PlgReference &other, PlgContext &context) const {
    context.CreateFrame();
    context.Set(self, other);
    return context.MergeDownFrame();
}

PlgReference PlgExpressionVariableName::RenameVars(const PlgReference &self, NameGeneratorFunction nameGenerator, SHashTable &existingVars) const {
    PlgReference newName = existingVars->FindItem(self, PlgUnbound);
    if (newName == PlgUnbound) {
        newName = PlgVariableName(nameGenerator());
        existingVars->AddItem(self, newName);
    }
    return newName;
}

//

IntelibTypeId PlgExpressionList::TypeId(&SExpression::TypeId, false);

// Disjunction

PlgReference PlgExpressionDisjunction::RenameVars(const PlgReference &self, NameGeneratorFunction nameGenerator, SHashTable &existingVars) const {
    SReference newList = *PTheEmptyList;
    for (SReference p = list; !p.IsEmptyList(); p = p.Cdr()) {
        PlgReference pref = p.Car();
        newList.AddAnotherItemToList(pref.RenameVars(nameGenerator, existingVars));
    }
    return PlgDisjunction(newList);
}

IntelibTypeId PlgExpressionDisjunction::TypeId(&PlgExpressionList::TypeId, false);

bool PlgExpressionDisjunction::Solve(const PlgReference &self, PlgExpressionContinuation &continuation) const {
    //TODO
    throw IntelibX_not_implemented();
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionDisjunction::TextRepresentation() const {
    return Join("; ", list);
}
#endif

PlgDisjunction operator | (const PlgReference &left, const PlgReference &right) {
    return PlgDisjunction((S| left, right ));
}

// Conjunction

IntelibTypeId PlgExpressionConjunction::TypeId(&PlgExpressionList::TypeId, false);

bool PlgExpressionConjunction::Solve(const PlgReference &self, PlgExpressionContinuation &continuation) const {
    //TODO
    throw IntelibX_not_implemented();
}

PlgReference PlgExpressionConjunction::RenameVars(const PlgReference &self, NameGeneratorFunction nameGenerator, SHashTable &existingVars) const {
    SReference newList = *PTheEmptyList;
    for (SReference p = list; !p.IsEmptyList(); p = p.Cdr()) {
        PlgReference pref = p.Car();
        newList.AddAnotherItemToList(pref.RenameVars(nameGenerator, existingVars));
    }
    return PlgConjunction(newList);
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionConjunction::TextRepresentation() const {
    return Join(", ", list);
}
#endif

PlgConjunction operator & (const PlgReference &left, const PlgReference &right) {
    return PlgConjunction((S| left, right ));
}
