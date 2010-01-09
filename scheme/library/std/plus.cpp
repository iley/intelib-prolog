//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/plus.cpp
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

DECLARE_CFUNCTION(SchFunctionPlus, -1, -1, "#<FUNCTION PLUS>", "+")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION
void SchFunctionPlus::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(paramsc==0) {
        lf.RegularReturn(0);
        return;
    }
    // The first argument determines whether we
    //          concatenate strings or sum numbers
    if(paramsv[0]->TermType().IsSubtypeOf(SExpressionString::TypeId)) {
        // We work with strings
        SString res = paramsv[0].GetString();
        for(int i = 1; i < paramsc; i++) {
            res += paramsv[i].GetString();
        }
        lf.RegularReturn(res);
    } else {
        // We work with numbers
        SchReference res = paramsv[0];
        for(int i = 1; i < paramsc; i++) {
            SchReference op = paramsv[i];
            if(LispUnifyNumbericTypes(res, op) == SExpressionInt::TypeId) {
                res = new SExpressionInt(res.GetInt() +
                                         paramsv[i].GetInt());
            } else {
                res = new SExpressionFloat(res.GetFloat() +
                                         paramsv[i].GetFloat());
            }
        }
        lf.RegularReturn(res);
    }
}

#endif

