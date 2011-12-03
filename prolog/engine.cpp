#include "engine.hpp"
#include "utils.hpp"
#include <stdio.h>
#include <stdlib.h>

// Context frame

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
    //TODO bool assignVars
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

PlgContext::Frame *PlgContext::CreateFrame() {
    Frame *oldFrame = top;
    top = new Frame(oldFrame);
    return oldFrame;
}

// Context

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

bool PlgContext::MergeDownFrame() {
    Frame *upperFrame = top;
    top = top->prev;
    INTELIB_ASSERT(top, IntelibX_unexpected_unbound_value());
    SExpressionHashTable::Iterator it(*upperFrame->table);

    bool result = true;
    SReference cons = it.GetNext();
    while (cons.GetPtr()) {
        PlgReference name = cons.Car();
        PlgReference value = cons.Cdr();

        if (Get(name) == PlgUnbound) {
            Set(name, value);
        } else if (!Get(name).Unify(value, *this)) {
            result = false;
            break;
        }

        cons = it.GetNext();
    }

    delete upperFrame;
    return result;
}

// Continuation

IntelibTypeId PlgExpressionContinuation::TypeId(&PlgExpression::TypeId, true);

PlgExpressionContinuation::PlgExpressionContinuation(PlgDatabase &db, const PlgReference &req)
    : PlgExpression(TypeId), database(db), choicePoints(*PTheEmptyList), request(req) {}

bool PlgExpressionContinuation::Next() {
    if (request != PlgUnbound) {
        PlgReference req = request;
        request = PlgUnbound;
        return req.Solve(*this);

    } else if (choicePoints.IsEmptyList()) {
        return false;

    } else {

        while (!choicePoints.IsEmptyList()) {
            PlgChoicePoint cp = choicePoints.Car();

            if(cp->Next(*this))
                return true;

            choicePoints = choicePoints.Cdr();
        }

        return false;
    }
}

void PlgExpressionContinuation::PushChoicePoint(const PlgReference &point) {
    //FIXME
    INTELIB_ASSERT(point->TermType().IsSubtypeOf(PlgExpressionChoicePoint::TypeId), IntelibX_not_a_prolog_choice_point(point));
    choicePoints = point.MakeCons(choicePoints);
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionContinuation::TextRepresentation() const {
    return "<PROLOG CONTINUATION>";
}
#endif

// Choice point
IntelibTypeId PlgExpressionChoicePoint::TypeId(&PlgExpression::TypeId, true);

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionChoicePoint::TextRepresentation() const {
    return "<PROLOG CHOICE POINT>";
}
#endif

bool PlgExpressionClauseChoicePoint::Next(PlgExpressionContinuation &continuation) {

    while (!pointer.IsEmptyList()) {
        PlgClause candidate = pointer.Car();

        continuation.Context().ReturnTo(frame);
        continuation.Context().CreateFrame();

        if ( frame->Evaluate(clause).Unify(frame->Evaluate(candidate->Head()), continuation.Context())
                && candidate->Body().Solve(continuation)) {
            return true;
        } else {
            continuation.Context().DropFrame();
        }

        pointer = pointer.Cdr();
    }

    return false;
}

IntelibTypeId PlgExpressionClauseChoicePoint::TypeId(&PlgExpressionChoicePoint::TypeId, true);

// Database
void PlgDatabase::Add(const PlgReference &clause) {
    clauses.AddAnotherItemToList(clause);
}

PlgContinuation PlgDatabase::Query(const PlgReference &request) {
    PlgContinuation cont(*this, request);
    cont->context.CreateFrame(); //???
    return cont;
}
