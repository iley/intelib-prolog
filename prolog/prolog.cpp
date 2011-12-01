#include "prolog.hpp"
#include "utils.hpp"

static SListConstructor S;

IntelibTypeId PlgExpression::TypeId(&SExpression::TypeId, true);

IntelibTypeId PlgClauseExpression::TypeId(&PlgExpression::TypeId, true);

bool PlgExpression::Unify(const PlgReference &other, PlgContext &context) {
    // TODO
    return false;
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpression::TextRepresentation() const { return "<PROLOG EXPRESSION>"; }
#endif

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

// Term

IntelibTypeId PlgTermExpression::TypeId(&PlgExpression::TypeId, false);

PlgTermExpression::PlgTermExpression(const PlgAtom &fn, const SReference &as) : functor(fn), args(as), arity(Length(as)) {}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgTermExpression::TextRepresentation() const { 
    return functor->TextRepresentation() + "(" + Join(", ", args) + ")";
}
#endif

// Atom

IntelibTypeId PlgAtomExpression::TypeId(&PlgExpression::TypeId, false);

PlgReference PlgAtom::operator() (const PlgReference &arg1) {
    return PlgTerm(*this, (S| arg1 ));
}

PlgReference PlgAtom::operator() (const PlgReference &arg1, const PlgReference &arg2) {
    return PlgTerm(*this, (S| arg1, arg2 ));
}

PlgReference PlgAtom::operator() (const PlgReference &arg1, const PlgReference &arg2, const PlgReference &arg3) {
    return PlgTerm(*this, (S| arg1, arg2, arg3 ));
}

// TODO more args

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgAtomExpression::TextRepresentation() const { return label->TextRepresentation(); }
#endif

// Variable Name

IntelibTypeId PlgVariableNameExpression::TypeId(&PlgAtomExpression::TypeId, false);



IntelibTypeId PlgListExpression::TypeId(&PlgExpression::TypeId, false);

// Disjunction

IntelibTypeId PlgDisjunctionExpression::TypeId(&PlgListExpression::TypeId, false);

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgDisjunctionExpression::TextRepresentation() const {
    return Join("; ", list);
}
#endif

PlgDisjunction operator | (const PlgReference &left, const PlgReference &right) {
    return PlgDisjunction((S| left, right ));
}

// Conjunction

IntelibTypeId PlgConjunctionExpression::TypeId(&PlgListExpression::TypeId, false);

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgConjunctionExpression::TextRepresentation() const {
    return Join(", ", list);
}
#endif

PlgConjunction operator & (const PlgReference &left, const PlgReference &right) {
    return PlgConjunction((S| left, right ));
}
