//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/lcm.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "std_inc.h"

DECLARE_CFUNCTION(SchFunctionLcm, -1, -1, "#<FUNCTION LCM>", "LCM")
DECLARE_CFUNCTION(SchFunctionGcd, -1, -1, "#<FUNCTION GCD>", "GCD")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION

static intelib_integer_t compute_gcd(intelib_integer_t a, intelib_integer_t b)
{
    if(b > a) b %= a;
    while(b != 0) {
        intelib_integer_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

static intelib_integer_t compute_lcm(intelib_integer_t a, intelib_integer_t b)
{
    return a / compute_gcd(a, b) * b;
}

typedef intelib_integer_t (*lcmgcd)(intelib_integer_t, intelib_integer_t);

static intelib_integer_t do_compute(int paramsc,
                                    const SReference paramsv[],
                                    int defval, lcmgcd fun)
{
    if(paramsc == 0) {
        return defval;
    }
    intelib_integer_t res = paramsv[0].GetInt();
    for(int i = 1; i < paramsc; i++)
        res = fun(res, paramsv[i].GetInt());
    return res < 0 ? -res : res;
}

void SchFunctionLcm::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    lf.RegularReturn(do_compute(paramsc, paramsv, 1, compute_lcm));
}

void SchFunctionGcd::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    lf.RegularReturn(do_compute(paramsc, paramsv, 0, compute_gcd));
}

#endif

