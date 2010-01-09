//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/isqrt.cpp
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

DECLARE_CFUNCTION(LFunctionIsqrt, 1, 1, "#<FUNCTION ISQRT>", "ISQRT")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionIsqrt::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    intelib_integer_t arg = paramsv[0].GetInt();
    INTELIB_ASSERT(arg>=0, IntelibX("ISQRT argument must be non-negative",
                                    paramsv[0]));
#if 0
    intelib_integer_t t = 1;
    while(t*t <= arg) t<<=1;
    t >>= 1;
#else
    intelib_integer_t t = 1 << (sizeof(intelib_integer_t)*8/2-1);
    while(t*t > arg) t >>= 1;
#endif
    intelib_integer_t res = t;
    t >>= 1;
    while(t) {
        res |= t;
        if(res*res > arg) res &= ~t;
        t >>= 1;
    }
    lf.RegularReturn(SReference(new SExpressionInt(res)));
}
#endif

