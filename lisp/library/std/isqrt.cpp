// +-------------------------------------------------------------------------+
// |               I__n__t__e__L__i__b           0.6.21 development          |
// | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
// |                                                                         |
// | This is free software. The library part is available under              |
// |                               GNU LESSER GENERAL PUBLIC LICENSE v.2.1.  |
// | GNU LGPL v2.1 is found in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
// |     Please see also docs/readme.txt and visit http://www.intelib.org    |
// |                                                                         |
// | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
// +-------------------------------------------------------------------------+




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

