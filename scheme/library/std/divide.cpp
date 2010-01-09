//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/divide.cpp
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

DECLARE_CFUNCTION(SchFunctionDivide, 1, -1, "#<FUNCTION DIVIDE>", "/")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION
void SchFunctionDivide::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SchReference res = paramsv[0];
    for(int i = 1; i < paramsc; i++) {
        SchReference op = paramsv[i];
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

