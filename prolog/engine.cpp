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

PlgContext::Frame *PlgContext::CurrentFrame() {
    return top;
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

IntelibTypeId PlgExpressionContinuation::TypeId;

bool PlgExpressionContinuation::Next() {
    if (choicePoints.IsEmptyList()) return false;

    PlgChoicePoint *cp = choicePoints.Car().DynamicCastGetPtr<PlgChoicePoint>();
    if (!cp->Next(*this))
        choicePoints = choicePoints.Cdr();

    //TODO
    return false;
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionContinuation::TextRepresentation() const {
    return "<INTELIB CONTINUATION>";
}
#endif

// Choice point

bool PlgClauseChoicePoint::Next(PlgContinuation &continuation) {
    //TODO
    return false;
}

// Database
void PlgDatabase::Add(const PlgReference &clause) {
    clauses.Append(clause);
}
