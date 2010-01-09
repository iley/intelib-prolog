//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/quotient.cpp
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

