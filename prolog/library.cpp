#include "library.hpp"
#include "utils.hpp"

static SListConstructor S;

bool PlgDefaultPredicate(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
    PlgClauseChoicePoint cp(PlgTerm(functor, args), cont);
    cont.PushChoicePoint(cp);
    return false; //to force backtracking
}

// Conjunction

bool PredicateConj(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
    PlgReference left = args.Car();
    PlgReference right = args.Cdr().Car();
    cont.PushQuery(right);
    cont.PushQuery(left);
    return true;
}

PlgAtom PlgAtomConj(", ", 2, PredicateConj, true);

// Disjunction

bool PredicateDisj(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
    PlgDisjChoicePoint cp(args, cont);
    cont.PushChoicePoint(cp);
    return false; //to force backtracking
}

PlgAtom PlgAtomDisj("; ", 2, PredicateDisj, true);

// "=" predicate

bool PredicateEquals(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
    PlgReference left = args.Car();
    PlgReference right = args.Cdr().Car();

    return left.Unify(right, cont.Context());
}

PlgAtom PlgAtomEquals("=", 2, PredicateEquals, true);

bool PredicateTrue(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
    return true;
}

PlgAtom PlgTrue("true", 0, PredicateTrue, false);