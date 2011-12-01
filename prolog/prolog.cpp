#include "prolog.hpp"
#include "utils.hpp"

IntelibTypeId PlgExpression::TypeId(&SExpression::TypeId, true);

bool PlgExpression::Unify(const PlgReference &other, PlgContext &context) {
    // TODO
    return false;
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpression::TextRepresentation() const { return "<PROLOG EXPRESSION>"; }
#endif

IntelibX_not_a_prolog_expression::IntelibX_not_a_prolog_expression(SReference a_param) 
    : IntelibX("Not a prolog expression", a_param) {}

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
        PlgClause *candidate = candidates.Car().DynamicCastGetPtr<PlgClause>();
        candidates = candidates.Cdr();
        if (clause->Unify(candidate->Head(), context)) {
            executionQueue.Append(candidate->Body());
            return true;
        }
    }

    return false;
}

IntelibX_not_a_prolog_atom::IntelibX_not_a_prolog_atom(SReference a_param) 
    : IntelibX("Not a prolog atom", a_param) {}

IntelibTypeId PlgAtomExpression::TypeId(&PlgExpression::TypeId, false);

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgAtomExpression::TextRepresentation() const { return label->TextRepresentation(); }
#endif

//IntelibTypeId PlgVariableName::TypeId(&PlgExpression::TypeId, false);

IntelibTypeId PlgTermExpression::TypeId(&PlgExpression::TypeId, false);

PlgTermExpression::PlgTermExpression(const PlgAtom &fn, const SReference &as) : functor(fn), args(as), arity(Length(as)) {}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgTermExpression::TextRepresentation() const { return functor->TextRepresentation() + "(" + Join(",", args) + ")"; }
#endif
