#ifndef INTELIB_PROLOG_LIBRARY_HPP_SENTRY
#define INTELIB_PROLOG_LIBRARY_HPP_SENTRY
#include "data.hpp"

bool PlgDefaultPredicate(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont);

namespace PlgStdLib {
    extern PlgAtom disjunction;
    extern PlgAtom conjunction;
    extern PlgAtom implication;
    extern PlgAtom unifies;
    extern PlgAtom truth;
    extern PlgAtom fail;
    extern PlgAtom not_unifies;
    extern PlgAtom cut;

    extern PlgAtom assert;
    extern PlgAtom asserta;
    extern PlgAtom assertz;

    // Integer arithmetic
    extern PlgAtom minus;
    extern PlgAtom plus;
    extern PlgAtom multiply;
    extern PlgAtom divide;
    extern PlgAtom reminder;
    extern PlgAtom is;
    extern PlgAtom int_equal;
    extern PlgAtom int_not_equal;
    extern PlgAtom int_less;
    extern PlgAtom int_less_or_equal;
    extern PlgAtom int_greater;
    extern PlgAtom int_greater_or_equal;

    extern PlgAtom nope; //not/1 in standard prolog

    extern PlgAtom append;
    extern PlgAtom length;
    extern PlgAtom member;
    extern PlgAtom nth;
    extern PlgAtom nth0;
    extern PlgAtom permutation;
    extern PlgAtom repeat;
    extern PlgAtom reverse;
    extern PlgAtom select;
}

inline PlgReference operator <<= (const PlgReference &left, const PlgReference &right)
{
    return PlgStdLib::implication(left, right);
}

inline PlgReference operator | (const PlgReference &left, const PlgReference &right)
{
    return PlgStdLib::disjunction(left, right);
}

inline PlgReference operator & (const PlgReference &left, const PlgReference &right)
{
    return PlgStdLib::conjunction(left, right);
}

inline PlgReference operator ^= (const PlgReference &left, const PlgReference &right)
{
    return PlgStdLib::unifies(left, right);
}

inline PlgReference operator - (const PlgReference &left, const PlgReference &right)
{
    return PlgStdLib::minus(left, right);
}

inline PlgReference operator + (const PlgReference &left, const PlgReference &right)
{
    return PlgStdLib::plus(left, right);
}

inline PlgReference operator * (const PlgReference &left, const PlgReference &right)
{
    return PlgStdLib::multiply(left, right);
}

inline PlgReference operator / (const PlgReference &left, const PlgReference &right)
{
    return PlgStdLib::divide(left, right);
}

inline PlgReference operator % (const PlgReference &left, const PlgReference &right)
{
    return PlgStdLib::reminder(left, right);
}

inline PlgReference operator < (const PlgReference &left, const PlgReference &right)
{
    return PlgStdLib::int_less(left, right);
}

inline PlgReference operator <= (const PlgReference &left, const PlgReference &right)
{
    return PlgStdLib::int_less_or_equal(left, right);
}

inline PlgReference operator > (const PlgReference &left, const PlgReference &right)
{
    return PlgStdLib::int_greater(left, right);
}

inline PlgReference operator >= (const PlgReference &left, const PlgReference &right)
{
    return PlgStdLib::int_greater_or_equal(left, right);
}

#endif
