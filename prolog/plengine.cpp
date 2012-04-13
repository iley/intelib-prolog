#include "../sexpress/sstring.hpp"
#include "plengine.hpp"
#include "plutils.hpp"
#include "library/library.hpp"

void Assert(const PlgReference &clause)
{
    clause.Head().Functor()->Add(clause);
}

void AssertA(const PlgReference &clause)
{
    clause.Head().Functor()->AddA(clause);
}

void AssertWithoutExpansion(const PlgReference &clause)
{
    clause.Head().Functor()->AddWithoutExpansion(clause);
}

class PlgExpressionSentenceMark : public PlgObject, public SExpression
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionSentenceMark(PlgExpressionChoicePoint *cp) : SExpression(TypeId), choicePoint(cp) {}

    PlgExpressionChoicePoint *ChoicePoint() const { return choicePoint; }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    SString TextRepresentation() const
    {
        return SString("<SENTENCE ") + SReference((long)choicePoint)->TextRepresentation() + ">";
    }
#endif

private:
    PlgExpressionChoicePoint *choicePoint;
};

IntelibTypeId PlgExpressionSentenceMark::TypeId(&SExpression::TypeId, false);

typedef GenericSReference<PlgExpressionSentenceMark, IntelibX_not_a_prolog_sentence_mark> PlgSentenceMark_Super;

class PlgSentenceMark : public PlgSentenceMark_Super
{
public:
    PlgSentenceMark(PlgExpressionChoicePoint *cp) : PlgSentenceMark_Super(new PlgExpressionSentenceMark(cp)) {}
    PlgSentenceMark(const SReference &sref) : PlgSentenceMark_Super(sref) {}
};

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

PlgExpressionContinuation::PlgExpressionContinuation(const PlgReference &req)
    : SExpression(TypeId), choicePoints(*(GetEmptyList()))
{
    queries = req.MakeCons(*(GetEmptyList()));
}

bool PlgExpressionContinuation::Next()
{
    if (queries.IsEmptyList() && !Backtrack())
        return false;

    while (!queries.IsEmptyList()) {
        PlgReference query = queries.Car();
        queries = queries.Cdr();

        query = query.Evaluate(context);

        // workaround for 0-arity predicates
        if (query->TermType() == PlgExpressionAtom::TypeId)
            query = PlgTerm(PlgAtom(query), *(GetEmptyList()));

        if (query->TermType() == PlgExpressionTerm::TypeId) {
            PlgTerm term = query;
            PlgPredicate pred = term->Functor().GetPredicate(term->Arity());
            INTELIB_ASSERT(pred.GetPtr(), IntelibX_unexpected_unbound_value());

            if (!pred->Apply(term->Functor(), term->Args(), *this) && !Backtrack())
                return false;
        }

        // if query is not atom/struct, just ignore it
    }

    return true;
}

PlgReference PlgExpressionContinuation::GetValue(const PlgReference &var)
{
    return var.Evaluate(context);
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
    choicePoints = *(GetEmptyList());
}

void PlgExpressionContinuation::Cut()
{
    // find latest sentence mark
    for (SReference q = queries; !q.IsEmptyList(); q = q.Cdr()) {
        if (q.Car()->TermType() == PlgExpressionSentenceMark::TypeId) {
            PlgSentenceMark mark = q.Car();

            // find choice point corresponding to current sentence start
            for (SReference cons = choicePoints; !cons.IsEmptyList(); cons = cons.Cdr()) {
                if (cons.Car().GetPtr() == mark->ChoicePoint()) {
                    // remove all choice points down to that choice point
                    choicePoints = cons.Cdr();
                    return;
                }
            }
            throw IntelibX("Choice point not found in Cut()");
        }
    }

    // remove all choice points if no sentence marks found
    choicePoints = *(GetEmptyList());
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

/*
// this may be very slow
bool PlgDatabase::Retract(const PlgReference &head, PlgContext &cont)
{
    PlgReference functor = head.Functor();
    SReference list = table->FindItem(functor, *(GetEmptyList()));
    int count = 0;

    SReference newList = *(GetEmptyList());

    for (SReference p = list; !p.IsEmptyList(); p = p.Cdr()) {
        PlgReference clause = p.Car();
        int frame = cont.NextFrame();
        if (clause.Head().Unify(head, cont)) {
            ++count;
        } else {
            newList.AddAnotherItemToList(clause);
        }
        cont.ReturnTo(frame);
    }

    if (count > 0) {
        table->AddItem(functor, newList);
        return true;
    } else {
        return false;
    }
}
*/

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
    return SString("<CHOICE POINT ") + SReference((long)this)->TextRepresentation() + ">";
}
#endif

bool PlgExpressionClauseChoicePoint::TryNext()
{
    INTELIB_ASSERT(pointer.GetPtr(), IntelibX_unexpected_unbound_value());
    while (!pointer.IsEmptyList()) {
        Restore();

        PlgTerm candidate = pointer.Car();
        pointer = pointer.Cdr();

        PlgReference head, body;
        if (candidate->Functor().IsEql(PlgStdLib::implication)) {
            head = candidate->Args().Car();
            body = candidate->Args().Cdr().Car();
        } else {
            head = candidate;
            body = PlgStdLib::truth;
        }

        SHashTable vars;
        if (clause.Unify(head.RenameVars(cont.context, vars), cont.context)) {
            cont.PushQuery(PlgSentenceMark(this));
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
