#include "../sexpress/sstring.hpp"
#include "engine.hpp"
#include "utils.hpp"
#include <stdio.h>

// Context

PlgReference PlgContext::Evaluate(const PlgReference &value) const {
    if (!value.GetPtr()) return value;

    PlgReference ref = value;

    while (ref.GetPtr() && ref->TermType() == PlgExpressionVariableIndex::TypeId) {
        ref = Get(ref);
    }
    
    if (ref.GetPtr() && ref->TermType() == PlgExpressionTerm::TypeId) {
        SReference resultArgs = *PTheEmptyList;
        PlgTerm term = ref;

        for (SReference p = term->Args(); !p.IsEmptyList(); p = p.Cdr()) {
            resultArgs.AddAnotherItemToList(Evaluate(p.Car()));
        }

        return PlgTerm(term->Functor(), resultArgs);
    } else if (ref.GetPtr()) {
        return ref;
    } else {
        return value;
    }
}

void PlgContext::ReturnTo(int pos, bool merge) {
    INTELIB_ASSERT(pos >= 0, IntelibX_bug());
    if (merge) {
        // merge down values of variables which will be dropped
        for (int i = 0; i < pos; ++i) {
            while (values[i]->TermType() == PlgExpressionVariableIndex::TypeId && indexValue(values[i]) >= pos) {
                values[i] = values[indexValue(values[i])];
            }
        }
    } else {
        // destroy bindings which will be invalid after return
        for (int i = 0; i < pos; ++i) {
            if (
                values[i].GetPtr()
                && values[i]->TermType() == PlgExpressionVariableIndex::TypeId
                && indexValue(values[i]) >= pos
            ) {
                values[i] = PlgUnbound;
            }
        }
    }

    top = pos;
}

int PlgContext::indexValue(const PlgReference &plgIndex) const {
    PlgExpressionVariableIndex *index = plgIndex.DynamicCastGetPtr<PlgExpressionVariableIndex>();
    INTELIB_ASSERT(index, IntelibX_bug()); //TODO: proper exception type
    return index->GetValue();
}

// Continuation

IntelibTypeId PlgExpressionContinuation::TypeId(&SExpression::TypeId, true);

PlgExpressionContinuation::PlgExpressionContinuation(PlgDatabase &db, const PlgReference &req)
    : SExpression(TypeId), database(db), choicePoints(*PTheEmptyList), queryVars(), request(req.RenameVars(context, queryVars)) {}

bool PlgExpressionContinuation::Next() {
    if (request != PlgUnbound) {
        bool result = request.Solve(*this);
        request = PlgUnbound;
        return result;
    } else {

        while (!choicePoints.IsEmptyList()) {
            PlgChoicePoint cp = choicePoints.Car();

            // now chice point calls cont.PopChoicePoint() by itself
            //choicePoints = choicePoints.Cdr();

            if(cp->Next(*this))
                return true;
        }

        return false;
    }
}

PlgReference PlgExpressionContinuation::GetValue(const PlgReference &var) {
    return context.Evaluate(var.RenameVars(context, queryVars));
}

void PlgExpressionContinuation::PushChoicePoint(const PlgReference &point) {
    INTELIB_ASSERT(point->TermType().IsSubtypeOf(PlgExpressionChoicePoint::TypeId), IntelibX_not_a_prolog_choice_point(point));
    choicePoints = point.MakeCons(choicePoints);
}

void PlgExpressionContinuation::PopChoicePoint() {
    INTELIB_ASSERT(!choicePoints.IsEmptyList(), IntelibX_bug());
    choicePoints = choicePoints.Cdr();
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionContinuation::TextRepresentation() const {
    return "<PROLOG CONTINUATION>";
}
#endif

// Choice point
IntelibTypeId PlgExpressionChoicePoint::TypeId(&SExpression::TypeId, true);

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionChoicePoint::TextRepresentation() const {
    return "<PROLOG CHOICE POINT>";
}
#endif

bool PlgExpressionClauseChoicePoint::Next(PlgExpressionContinuation &cont) {
    while (!pointer.IsEmptyList()) {
        cont.Context().ReturnTo(contextPos);
        
        SHashTable renameTable;
        PlgClause candidate = PlgReference(pointer.Car()).RenameVars(cont.Context(), renameTable);
        pointer = pointer.Cdr();

        if (clause.Unify(candidate->Head(), cont.Context()) && candidate->Body().Solve(cont)) {
            return true;
        }
    }

    // if failed then cleanup and return false
    cont.Context().ReturnTo(contextPos);
    cont.PopChoicePoint();
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
