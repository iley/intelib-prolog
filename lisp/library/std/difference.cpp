//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/difference.cpp
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

DECLARE_CFUNCTION(LFunctionDifference, 1, -1, "#<FUNCTION DIFFERENCE>", "-")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionDifference::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(paramsc == 1) {
        // special case - unary minus
        if(paramsv[0]->TermType() == SExpressionInt::TypeId) {
            lf.RegularReturn(
                LReference(new SExpressionInt(-paramsv[0].GetInt())));
        } else
            if(paramsv[0]->TermType() == SExpressionFloat::TypeId) {
                lf.RegularReturn(
                    LReference(new SExpressionFloat(-paramsv[0].GetFloat())));
            }
        return;
    }
    // Ok, no more special cases
    LReference res = paramsv[0];
    for(int i = 1; i < paramsc; i++) {
        LReference op = paramsv[i];
        if(LispUnifyNumbericTypes(res, op) == SExpressionInt::TypeId) {
            res = new SExpressionInt(res.GetInt() - paramsv[i].GetInt());
        } else {
            res = new SExpressionFloat(res.GetFloat() - paramsv[i].GetFloat());
        }
    }
    lf.RegularReturn(res);
}

#endif

