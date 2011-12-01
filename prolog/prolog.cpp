#include "prolog.hpp"

IntelibTypeId PlgExpression::TypeId(&SExpression::TypeId, true);

bool PlgExpression::Unify(const PlgReference &other, PlgContext &context) {
    // TODO
    return false;
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpression::TextRepresentation() const { return "<PROLOG EXPRESSION>"; }
#endif

IntelibX_not_a_prolog_expression::IntelibX_not_a_prolog_expression(SReference a_param) 
    : IntelibX("Not a prolog expression", a_param) {}

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

    PlgExpression *nextClause = nextClauseRef.DynamicCastGetPtr<PlgExpression>();

    //switch by clause type
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

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgAtom::TextRepresentation() const { return label->TextRepresentation(); }
#endif
