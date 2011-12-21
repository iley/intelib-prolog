#ifndef INTELIB_PROLOG_LIBRARY_HPP_SENTRY
#define INTELIB_PROLOG_LIBRARY_HPP_SENTRY
#include "data.hpp"

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

#endif
