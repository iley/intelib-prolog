//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/times.cpp
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

DECLARE_CFUNCTION(LFunctionTimes, -1, -1, "#<FUNCTION TIMES>", "*")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionTimes::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(paramsc==0) {
        lf.RegularReturn(1);
        return;
    }
    LReference res = paramsv[0];
    for(int i = 1; i < paramsc; i++) {
        LReference op = paramsv[i];
        if(LispUnifyNumbericTypes(res, op) == SExpressionInt::TypeId) {
            res = new SExpressionInt(res.GetInt() * paramsv[i].GetInt());
        } else {
            res = new SExpressionFloat(res.GetFloat() * paramsv[i].GetFloat());
        }
    }
    lf.RegularReturn(res);
}

#endif

