#ifndef INTELIB_PROLOG_LIBRARY_HPP_SENTRY
#define INTELIB_PROLOG_LIBRARY_HPP_SENTRY
#include "data.hpp"

bool PlgDefaultPredicate(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont);

// Disjunction
extern PlgAtom PlgAtomDisj;

inline PlgReference operator | (const PlgReference &left, const PlgReference &right) {
    return PlgAtomDisj(left, right);
}

// Conjunction

extern PlgAtom PlgAtomConj;

inline PlgReference operator & (const PlgReference &left, const PlgReference &right) {
    return PlgAtomConj(left, right);
}

// Unification term

extern PlgAtom PlgAtomEquals;
inline PlgReference operator ^= (const PlgReference &left, const PlgReference &right) {
    return PlgAtomEquals(left, right);
}

extern PlgAtom PlgTrue;
extern PlgAtom PlgNotUnifies;

// Integer arithmetic

extern PlgAtom PlgAtomMinus;

inline PlgReference operator - (const PlgReference &left, const PlgReference &right) {
    return PlgAtomMinus(left, right);
}

extern PlgAtom PlgAtomPlus;

inline PlgReference operator + (const PlgReference &left, const PlgReference &right) {
    return PlgAtomPlus(left, right);
}

extern PlgAtom PlgAtomMultiply;

inline PlgReference operator * (const PlgReference &left, const PlgReference &right) {
    return PlgAtomMultiply(left, right);
}

extern PlgAtom PlgAtomDivide;

inline PlgReference operator / (const PlgReference &left, const PlgReference &right) {
    return PlgAtomDivide(left, right);
}

extern PlgAtom PlgAtomIs;

extern PlgAtom PlgAtomNumericEq;
extern PlgAtom PlgAtomNumericNe;

extern PlgAtom PlgAtomNumericLess;

inline PlgReference operator < (const PlgReference &left, const PlgReference &right) {
    return PlgAtomNumericLess(left, right);
}

extern PlgAtom PlgAtomNumericLessOrEqual;

inline PlgReference operator <= (const PlgReference &left, const PlgReference &right) {
    return PlgAtomNumericLessOrEqual(left, right);
}

extern PlgAtom PlgAtomNumericGreater;

inline PlgReference operator > (const PlgReference &left, const PlgReference &right) {
    return PlgAtomNumericGreater(left, right);
}

extern PlgAtom PlgAtomNumericGreaterOrEqual;

inline PlgReference operator >= (const PlgReference &left, const PlgReference &right) {
    return PlgAtomNumericGreaterOrEqual(left, right);
}

#endif
