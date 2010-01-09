//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/append.cpp
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

DECLARE_CFUNCTION(LFunctionAppend, -1, -1, "#<FUNCTION APPEND>", "APPEND")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
static LReference do_append(SExpressionCons *dp, LReference &last)
{
    SExpressionCons *next_dp = dp->Cdr().DynamicCastGetPtr<SExpressionCons>();
    if(next_dp) {
        return LReference(new SExpressionCons(dp->Car(),
                      do_append(next_dp, last)));
    } else {
        return LReference(new SExpressionCons(dp->Car(), last));
    }
}
void LFunctionAppend::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(paramsc == 0) {
        lf.RegularReturn(*PTheEmptyList);
        return;
    }
    LReference res = paramsv[paramsc-1];
    for(int i = paramsc-2; i>=0; i--) {
        SExpressionCons *dp = paramsv[i].DynamicCastGetPtr<SExpressionCons>();
        if(!dp) {
            if(paramsv[i].IsEmptyList())
                continue;
            else
                throw IntelibX_not_a_list(paramsv[i]);
        }
        res = do_append(dp, res);
    }
    lf.RegularReturn(res);
}

#endif

