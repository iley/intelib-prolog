#include "../sexpress/sstring.hpp"
#include "engine.hpp"
#include "utils.hpp"
#include <stdio.h>

const char *NameGenerator() {
    static int count = 0;
    static char buffer[32];

    ++count;
    sprintf(buffer, "_%d", count);
    return buffer;
}

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
    Frame *oldTop = top;
    top = new Frame(oldTop);

    if (oldTop)
        oldTop->next = top;
    else
        bottom = top;

    return oldTop;
}

// Context

PlgContext::~PlgContext() {
    Frame *nextFrame;
    for(Frame *frame = top; frame; frame = nextFrame) {
        nextFrame = frame->prev;
        delete frame;
    }
    top = bottom = 0;
}

void PlgContext::ReturnTo(Frame *frame, bool keepValues) {
    while (top != frame)
        DropFrame(keepValues);
}

void PlgContext::DropFrame(bool keepValues) {
    INTELIB_ASSERT(top && top != bottom, IntelibX_unexpected_unbound_value());
    Frame *droppedFrame = top;
    top = top->prev;

    if (keepValues) {
        top->Apply(*droppedFrame);
    }

    top->next = 0;

    delete droppedFrame;
}

bool PlgContext::MergeDownFrame() {
    Frame *upperFrame = top;
    top = top->prev;
    INTELIB_ASSERT(top && top != bottom, IntelibX_unexpected_unbound_value());
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

    top->next = 0;

    delete upperFrame;
    return result;
}

// Continuation

IntelibTypeId PlgExpressionContinuation::TypeId(&SExpression::TypeId, true);

PlgExpressionContinuation::PlgExpressionContinuation(PlgDatabase &db, const PlgReference &req)
    : SExpression(TypeId), database(db), choicePoints(*PTheEmptyList), request(req) {}

bool PlgExpressionContinuation::Next() {
    if (request != PlgUnbound) {
        PlgReference req = request;
        request = PlgUnbound;
        return req.Solve(*this);

    } else {

        while (!choicePoints.IsEmptyList()) {
            PlgChoicePoint cp = choicePoints.Car();
            choicePoints = choicePoints.Cdr();

            if(cp->Next(*this))
                return true;
        }

        return false;
    }
}

PlgReference PlgExpressionContinuation::GetValue(const PlgReference &var) const {
    INTELIB_ASSERT(var->TermType() == PlgExpressionVariableName::TypeId, IntelibX_not_a_prolog_variable_name(var));
    PlgReference value = var;
    for (PlgContext::Frame *frame = context.Bottom()->Next(); frame; frame = frame->Next()) {
        PlgReference binding = frame->Evaluate(value);
        if (binding != PlgUnbound)
            value = binding;
    }

    // return original variable name if the variable is unbound
    if (value->TermType() == PlgExpressionVariableName::TypeId)
        return var;
    else
        return value;
}

void PlgExpressionContinuation::PushChoicePoint(const PlgReference &point) {
    INTELIB_ASSERT(point->TermType().IsSubtypeOf(PlgExpressionChoicePoint::TypeId), IntelibX_not_a_prolog_choice_point(point));
    choicePoints = point.MakeCons(choicePoints);
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionContinuation::TextRepresentation() const {
    return "<PROLOG CONTINUATION>"; //FIXME
}
#endif

// Choice point
IntelibTypeId PlgExpressionChoicePoint::TypeId(&SExpression::TypeId, true);

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionChoicePoint::TextRepresentation() const {
    return "<PROLOG CHOICE POINT>";
}
#endif

bool PlgExpressionClauseChoicePoint::Next(PlgExpressionContinuation &continuation) {
    SHashTable renameTable;

    while (!pointer.IsEmptyList()) {
        PlgClause candidate = PlgReference(pointer.Car()).RenameVars(NameGenerator, renameTable);
        pointer = pointer.Cdr();

        continuation.Context().ReturnTo(frame);
        continuation.Context().CreateFrame();

        if ( frame->Evaluate(clause).Unify(frame->Evaluate(candidate->Head()), continuation.Context())
                && candidate->Body().Solve(continuation)) {
            return true;
        } else {
            continuation.Context().DropFrame();
        }
    }
    continuation.Context().ReturnTo(frame);

    return false;
}

IntelibTypeId PlgExpressionClauseChoicePoint::TypeId(&PlgExpressionChoicePoint::TypeId, true);

// Database
void PlgDatabase::Add(const PlgReference &clause) {
    clauses.AddAnotherItemToList(clause);
}

PlgContinuation PlgDatabase::Query(const PlgReference &request) {
    PlgContinuation cont(*this, request);
    return cont;
}
