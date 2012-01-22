#include "../sexpress/sstring.hpp"
#include "engine.hpp"
#include "utils.hpp"
#include "library.hpp"

// Context

void PlgContext::ReturnTo(int pos, bool merge)
{
    INTELIB_ASSERT(pos >= 0, IntelibX_bug());
    if (!merge) {
        // destroy bindings which will be invalid after return
        for (int i = top; i > pos; --i) {
            for (SReference p = frameVars[i]; !p.IsEmptyList(); p = p.Cdr()) {
                PlgReference var = p.Car();
                values->RemoveItem(var);
            }
        }
    }

    top = pos;
}

// Continuation

IntelibTypeId PlgExpressionContinuation::TypeId(&SExpression::TypeId, true);

PlgExpressionContinuation::PlgExpressionContinuation(PlgDatabase &db, const PlgReference &req)
    : SExpression(TypeId), database(db), choicePoints(*PTheEmptyList), queryVars(), queryVarsSaved(false)
{
    queries = req.MakeCons(*PTheEmptyList);
}

bool PlgExpressionContinuation::Next()
{
    if (queries.IsEmptyList() && !Backtrack())
        return false;

    while (!queries.IsEmptyList()) {
        SHashTable vars;

        PlgReference query = queries.Car();
        queries = queries.Cdr();

        if (queryVarsSaved) {
            query = query.RenameVars(context, vars).Evaluate(context);
        } else {
            query = query.RenameVars(context, queryVars).Evaluate(context);
            queryVarsSaved = true;
        }

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

PlgReference PlgExpressionContinuation::GetValue(const PlgReference &var)
{
    return var.RenameVars(context, queryVars).Evaluate(context);
}

void PlgExpressionContinuation::PushChoicePoint(const PlgReference &point)
{
    INTELIB_ASSERT(point->TermType().IsSubtypeOf(PlgExpressionChoicePoint::TypeId), IntelibX_not_a_prolog_choice_point(point));
    choicePoints = point.MakeCons(choicePoints);
}

void PlgExpressionContinuation::PopChoicePoint()
{
    INTELIB_ASSERT(!choicePoints.IsEmptyList(), IntelibX_bug());
    choicePoints = choicePoints.Cdr();
}

void PlgExpressionContinuation::ResetChoicePoints()
{
    choicePoints = *PTheEmptyList;
}

void PlgExpressionContinuation::PushQuery(const PlgReference &query)
{
    queries = query.MakeCons(queries);
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionContinuation::TextRepresentation() const
{
    return "<PROLOG CONTINUATION>";
}
#endif

bool PlgExpressionContinuation::Backtrack()
{
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
void PlgDatabase::AddA(const PlgReference &ref)
{
    PlgReference clause = ExpandTerm(Clause(ref));
    PlgReference functor = clause.Head().Functor();
    SReference list = table->FindItem(functor, *PTheEmptyList);
    table->AddItem(functor, clause ^ list);
}

void PlgDatabase::Add(const PlgReference &ref)
{
    return AddWithoutExpansion(ExpandTerm(ref));
}

void PlgDatabase::AddWithoutExpansion(const PlgReference &ref)
{
    PlgReference clause = Clause(ref);
    PlgReference functor = ref.Head().Functor();
    SReference list = table->FindItem(functor, *PTheEmptyList);
    table->AddItem(functor, list.AddAnotherItemToList(clause));
}

PlgContinuation PlgDatabase::Query(const PlgReference &request)
{
    PlgContinuation cont(*this, request);
    return cont;
}

PlgReference PlgDatabase::Clause(const PlgReference &ref) const
{
    if (ref->TermType() == PlgExpressionTerm::TypeId)
        return ref;
    else
        return PlgTerm(ref, *PTheEmptyList);
}

PlgReference PlgDatabase::ExpandTerm(const PlgReference &ref)
{
    using PlgStdLib::expand_term;
    PlgVariableName X("X");
    PlgContinuation cont = Query(expand_term(ref, X));
    bool result = cont->Next();
    INTELIB_ASSERT(result, IntelibX_bug());
    return cont->GetValue(X);
}

// Choice point
IntelibTypeId PlgExpressionChoicePoint::TypeId(&SExpression::TypeId, true);

void PlgExpressionChoicePoint::Restore()
{
    cont.context.ReturnTo(contextPos);
    cont.queries = queries;
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionChoicePoint::TextRepresentation() const
{
    return "<PROLOG CHOICE POINT>";
}
#endif

bool PlgExpressionClauseChoicePoint::TryNext()
{
    while (!pointer.IsEmptyList()) {
        SHashTable vars;
        Restore();

        PlgTerm candidate = pointer.Car();
        pointer = pointer.Cdr();

        PlgReference head, body;
        if (candidate->Functor() == PlgStdLib::implication) {
            head = candidate->Args().Car();
            body = candidate->Args().Cdr().Car();
        } else {
            head = candidate;
            body = PlgStdLib::truth;
        }

        if (clause.Unify(head.RenameVars(cont.context, vars), cont.context)) {
            //TODO: method PushQuery
            cont.PushQuery(body.RenameVars(cont.context, vars));
            return true;
        }
    }

    return false;
}

IntelibTypeId PlgExpressionClauseChoicePoint::TypeId(&PlgExpressionChoicePoint::TypeId, true);

IntelibTypeId PlgExpressionDisjChoicePoint::TypeId(&PlgExpressionChoicePoint::TypeId, true);

bool PlgExpressionDisjChoicePoint::TryNext()
{
    if (variants.IsEmptyList())
        return false;

    Restore();

    PlgReference variant = variants.Car();
    variants = variants.Cdr();
    cont.PushQuery(variant);
    return true;
}
