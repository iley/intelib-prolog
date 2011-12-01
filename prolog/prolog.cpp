#include "prolog.hpp"
#include "utils.hpp"

IntelibTypeId PlgExpression::TypeId(&SExpression::TypeId, true);

bool PlgExpression::Unify(const PlgReference &other, PlgContext &context) {
    // TODO
    return false;
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpression::TextRepresentation() const { return "<PROLOG EXPRESSION>"; }
#endif

void PlgDatabase::Add(const PlgReference &clause) {
    clauses = clauses.MakeCons(clause);
}

bool PlgContinuation::Next() {
    while (queue.IsEmpty()) {
        if (choicePoints.IsEmptyList()) return false;

        PlgChoicePoint *cp = choicePoints.Car().DynamicCastGetPtr<PlgChoicePoint>();
        if (!cp->Next(context, queue))
            choicePoints = choicePoints.Cdr();
    }

    SReference nextClauseRef = queue.Car();
    queue.RemoveFirst();

    //PlgExpression *nextClause = nextClauseRef.DynamicCastGetPtr<PlgExpression>();

    //switch by clause type

    //TODO
    return false;
}

bool PlgClauseChoicePoint::Next(PlgContext &context, SQueue &executionQueue) {
    while (!candidates.IsEmptyList()) {
        PlgClause *candidate = candidates.Car().DynamicCastGetPtr<PlgClause>();
        candidates = candidates.Cdr();
        if (clause->Unify(candidate->Head(), context)) {
            executionQueue.Append(candidate->Body());
            return true;
        }
    }

    return false;
}

IntelibTypeId PlgAtomExpression::TypeId(&PlgExpression::TypeId, false);

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgAtomExpression::TextRepresentation() const { return label->TextRepresentation(); }
#endif

IntelibTypeId PlgVariableNameExpression::TypeId(&PlgAtomExpression::TypeId, false);

IntelibTypeId PlgTermExpression::TypeId(&PlgExpression::TypeId, false);

PlgTermExpression::PlgTermExpression(const PlgAtom &fn, const SReference &as) : functor(fn), args(as), arity(Length(as)) {}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgTermExpression::TextRepresentation() const { return functor->TextRepresentation() + "(" + Join(",", args) + ")"; }
#endif
