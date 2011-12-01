#include "engine.hpp"

void PlgContext::Frame::Set(const PlgVariableName &name, const PlgReference &value) {
    table->AddItem(name, value);
}

PlgReference PlgContext::Frame::Get(const PlgVariableName &name) {
    return table->FindItem(name, name);
}

void PlgContext::Set(const PlgVariableName &name, const PlgReference &value) {
    INTELIB_ASSERT(top, IntelibX_unexpected_unbound_value());
    top->Set(name, value);
}

PlgReference PlgContext::Get(const PlgVariableName &name) {
    INTELIB_ASSERT(top, IntelibX_unexpected_unbound_value());
    return top->Get(name);
}



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

