#include "engine.hpp"
#include "utils.hpp"
#include <stdio.h>
#include <stdlib.h>

void PlgContext::Frame::Set(const PlgReference &name, const PlgReference &value) {
    table->AddItem(name, value);
}

PlgReference PlgContext::Frame::Get(const PlgReference &name) const {
    return table->FindItem(name, PlgUnbound);
}

void PlgContext::Frame::Apply(const Frame &droppedFrame) {
    SExpressionHashTable::Iterator it(*table);

    SReference cons = it.GetNext();
    while (cons.GetPtr()) {
        PlgReference name = cons.Car();
        PlgReference value = cons.Cdr();

        cons.Cdr() = droppedFrame.Evaluate(value);

        cons = it.GetNext();
    }
}

PlgReference PlgContext::Frame::Evaluate(const PlgReference &value) const {
    if (value->TermType() == PlgExpressionVariableName::TypeId) {
        PlgReference result = Get(value);
        if (result == PlgUnbound)
            return value;
        else
            return result;
    } else if (value->TermType() == PlgExpressionTerm::TypeId) {
        PlgExpressionTerm *term = value.SimpleCastGetPtr<PlgExpressionTerm>();
        SReference resultArgs = *PTheEmptyList;

        for (SReference p = term->Args(); !p.IsEmptyList(); p = p.Cdr()) {
            resultArgs.AddAnotherItemToList(Evaluate(p.Car()));
        }

        return PlgTerm(term->Functor(), resultArgs);
    } else {
        return value;
    }
}

void PlgContext::Set(const PlgReference &name, const PlgReference &value) {
    INTELIB_ASSERT(top, IntelibX_unexpected_unbound_value());
    top->Set(name, value);
}

PlgReference PlgContext::Get(const PlgReference &name) const {
    INTELIB_ASSERT(top, IntelibX_unexpected_unbound_value());
    return top->Get(name);
}

PlgContext::Frame *PlgContext::CreateFrame() {
    Frame *oldFrame = top;
    top = new Frame(oldFrame);
    return oldFrame;
}

PlgContext::Frame *PlgContext::CurrentFrame() {
    return top;
}

void PlgContext::ReturnTo(Frame *frame, bool keepValues) {
    while (top != frame)
        DropFrame(keepValues);
}

void PlgContext::DropFrame(bool keepValues) {
    INTELIB_ASSERT(top, IntelibX_unexpected_unbound_value());
    Frame *droppedFrame = top;
    top = top->Prev();

    if (keepValues) {
        top->Apply(*droppedFrame);
    }

    delete droppedFrame;
}

void PlgDatabase::Add(const PlgReference &clause) {
    clauses.Append(clause);
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
        PlgExpressionClause *candidate = candidates.Car().DynamicCastGetPtr<PlgExpressionClause>();
        candidates = candidates.Cdr();
        if (clause->Unify(candidate->Head(), context)) {
            executionQueue.Append(candidate->Body());
            return true;
        }
    }

    return false;
}

