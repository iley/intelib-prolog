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

DECLARE_CFUNCTION(LFunctionQuotient, 1, -1, "#<FUNCTION QUOTIENT>", "/")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionQuotient::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    LReference res = paramsv[0];
    for(int i = 1; i < paramsc; i++) {
        LReference op = paramsv[i];
        if(LispUnifyNumbericTypes(res, op) == SExpressionInt::TypeId) {
            int i1 = res.GetInt();
            int i2 = paramsv[i].GetInt();
            if(i1 % i2 == 0) {
                // we can keep within integers
                res = new SExpressionInt(i1/i2);
            } else {
                intelib_float_t fl = i1;
                res = new SExpressionFloat(fl/i2);
            }
        } else {
            res = new SExpressionFloat(res.GetFloat() / paramsv[i].GetFloat());
        }
    }
    lf.RegularReturn(res);
}

#endif

