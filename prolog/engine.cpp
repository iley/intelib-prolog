#include "engine.hpp"

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
        PlgClauseExpression *candidate = candidates.Car().DynamicCastGetPtr<PlgClauseExpression>();
        candidates = candidates.Cdr();
        if (clause->Unify(candidate->Head(), context)) {
            executionQueue.Append(candidate->Body());
            return true;
        }
    }

    return false;
}

