#ifndef INTELIB_PROLOG_LIBRARY_HPP_SENTRY
#define INTELIB_PROLOG_LIBRARY_HPP_SENTRY
#include "../plgdata.hpp"
#include "../plgengine.hpp"
#include "io.hpp"
#include "trace.hpp"

// generated modules
#include "grammar.hpp"
#include "output.hpp"

bool PlgDefaultPredicate(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont);

namespace PlgStdLib {
    extern PlgAnonymousVariable _;

    extern PlgAtom action; // {} atom, for DCGs
    extern PlgAtom dcg_translate_rule;
    extern PlgAtom phrase;

    extern PlgAtom listing;
    extern PlgAtom error;

    extern PlgAtom disjunction;
    extern PlgAtom conjunction;
    extern PlgAtom implication;
    extern PlgAtom rightarrow;
    extern PlgAtom unifies;
    extern PlgAtom truth; //true in standard prolog
    extern PlgAtom fail;
    extern PlgAtom not_unifies;
    extern PlgAtom cut;
    extern PlgAtom univ;

    extern PlgAtom assert;
    extern PlgAtom asserta;
    extern PlgAtom assertz;
    extern PlgAtom retract;

    extern PlgAtom expand_term;
    extern PlgAtom term_expansion;
    extern PlgAtom goal_expansion;

    // Integer arithmetic
    extern PlgAtom minus;
    extern PlgAtom plus;
    extern PlgAtom multiply;
    extern PlgAtom divide;
    extern PlgAtom reminder;
    extern PlgAtom power;
    extern PlgAtom is;
    extern PlgAtom int_equal;
    extern PlgAtom int_not_equal;
    extern PlgAtom int_less;
    extern PlgAtom int_less_or_equal;
    extern PlgAtom int_greater;
    extern PlgAtom int_greater_or_equal;

    extern PlgAtom atom;
    extern PlgAtom atomic;
    extern PlgAtom compound;
    extern PlgAtom integer;
    extern PlgAtom nonvar;
    extern PlgAtom var;

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

inline PlgReference operator << (const PlgReference &left, const PlgReference &right)
{
    return PlgStdLib::implication(left, right);
}

inline void operator <<=(const PlgReference &left, const PlgReference &right)
{
    left.Functor()->Add(left << right);
}

inline void operator <<=(const PlgReference &left, bool value)
{
    if (value)
        return left <<= PlgStdLib::truth;
    else
        throw IntelibX_not_implemented();
}

inline void operator *(const PlgReference &fact)
{
    Assert(fact);
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

inline PlgReference operator - (const PlgReference &op)
{
    return PlgStdLib::minus(op);
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

inline PlgReference operator >>= (const PlgReference &left, const PlgReference &right)
{
    return PlgStdLib::rightarrow(left, right);
}

#endif
