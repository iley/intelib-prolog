#include "library.hpp"
#include "utils.hpp"

static SListConstructor S;

// Conjunction

bool PredicateConj(const SReference &args, PlgExpressionContinuation &cont) {
    PlgReference left = args.Car();
    PlgReference right = args.Cdr().Car();
    cont.PushQuery(right);
    cont.PushQuery(left);
    return true;
}

PlgAtom PlgAtomConj(", ", 2, PredicateConj, true);

// Disjunction

bool PredicateDisj(const SReference &args, PlgExpressionContinuation &cont) {
    throw IntelibX_not_implemented();
}

PlgAtom PlgAtomDisj("; ", 2, PredicateDisj, true);

// "=" predicate

bool PredicateEquals(const SReference &args, PlgExpressionContinuation &cont) {
    PlgReference left = args.Car();
    PlgReference right = args.Cdr().Car();

    return left.Unify(right, cont.Context());
}

PlgAtom PlgAtomEquals("=", 2, PredicateEquals, true);

bool PredicateTrue(const SReference &args, PlgExpressionContinuation &cont) {
    return true;
}

PlgAtom PlgTrue("true", 0, PredicateTrue, false);