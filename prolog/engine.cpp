#include "../sexpress/sstring.hpp"
#include "engine.hpp"
#include "utils.hpp"
#include "library.hpp"

// Context

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
        
        for (int i = top-1; i >= pos; --i) {
            PlgReference backlink = backlinks[i];
            if (backlink.GetPtr()) {
                values[indexValue(backlink)] = PlgUnbound;
                backlinks[i] = PlgUnbound;
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
    : SExpression(TypeId), database(db), choicePoints(*PTheEmptyList), queryVars()  
{
    queries = req.RenameVars(context, queryVars).MakeCons(*PTheEmptyList);
}

bool PlgExpressionContinuation::Next() {
    if (queries.IsEmptyList() && !Backtrack())
        return false;

    while (!queries.IsEmptyList()) {
        SHashTable vars;

        PlgReference query = queries.Car();
        queries = queries.Cdr();

        query = query.RenameVars(context, vars).Evaluate(context);

        // workaround for 0-arity predicates
        if (query->TermType() == PlgExpressionAtom::TypeId)
            query = PlgTerm(PlgAtom(query), *PTheEmptyList);

        if (query->TermType() == PlgExpressionTerm::TypeId) {
            PlgTerm term = query;
            PlgPredicate pred = term->Functor()->GetPredicate(term->Arity());
            INTELIB_ASSERT(pred.GetPtr(), IntelibX_unexpected_unbound_value());

            if (!pred->Apply(term->Functor(), term->Args(), *this) && !Backtrack())
                return false;
        }
    }

    return true;
}

PlgReference PlgExpressionContinuation::GetValue(const PlgReference &var) {
    return var.RenameVars(context, queryVars).Evaluate(context);
}

void PlgExpressionContinuation::PushChoicePoint(const PlgReference &point) {
    INTELIB_ASSERT(point->TermType().IsSubtypeOf(PlgExpressionChoicePoint::TypeId), IntelibX_not_a_prolog_choice_point(point));
    choicePoints = point.MakeCons(choicePoints);
}

void PlgExpressionContinuation::PopChoicePoint() {
    INTELIB_ASSERT(!choicePoints.IsEmptyList(), IntelibX_bug());
    choicePoints = choicePoints.Cdr();
}

void PlgExpressionContinuation::PushQuery(const PlgReference &query) {
    queries = query.MakeCons(queries);
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionContinuation::TextRepresentation() const {
    return "<PROLOG CONTINUATION>";
}
#endif

bool PlgExpressionContinuation::Backtrack() {
    while (!choicePoints.IsEmptyList()) {
        PlgChoicePoint cp = choicePoints.Car();
        if (cp->TryNext())
            return true;
        else
            choicePoints = choicePoints.Cdr();
    }
    return false;
}

// Database
void PlgDatabase::Add(const PlgReference &ref) {
    if (ref->TermType() == PlgExpressionClause::TypeId)
        clauses.AddAnotherItemToList(ref);
    else if (ref->TermType() == PlgExpressionTerm::TypeId)
        clauses.AddAnotherItemToList(PlgClause(ref, PlgTrue));
    else if (ref->TermType() == PlgExpressionAtom::TypeId)
        clauses.AddAnotherItemToList(PlgClause(PlgTerm(ref, *PTheEmptyList), PlgTrue));
}

PlgContinuation PlgDatabase::Query(const PlgReference &request) {
    PlgContinuation cont(*this, request);
    return cont;
}

// Choice point
IntelibTypeId PlgExpressionChoicePoint::TypeId(&SExpression::TypeId, true);

void PlgExpressionChoicePoint::Restore() {
    cont.context.ReturnTo(contextPos);
    cont.queries = queries;
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionChoicePoint::TextRepresentation() const {
    return "<PROLOG CHOICE POINT>";
}
#endif

bool PlgExpressionClauseChoicePoint::TryNext() {
    while (!pointer.IsEmptyList()) {
        SHashTable vars;
        Restore();

        PlgClause candidate = pointer.Car();
        pointer = pointer.Cdr();

        if (clause.Unify(candidate->Head().RenameVars(cont.context, vars), cont.context)) {
            //TODO: method PushQuery
            cont.PushQuery(candidate->Body().RenameVars(cont.context, vars));
            return true;
        }
    }

    return false;
}

IntelibTypeId PlgExpressionClauseChoicePoint::TypeId(&PlgExpressionChoicePoint::TypeId, true);

IntelibTypeId PlgExpressionDisjChoicePoint::TypeId(&PlgExpressionChoicePoint::TypeId, true);

bool PlgExpressionDisjChoicePoint::TryNext() {
    if (variants.IsEmptyList())
        return false;

    Restore();

    PlgReference variant = variants.Car();
    variants = variants.Cdr();
    cont.PushQuery(variant);
    return true;
}