//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/length.cpp
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

DECLARE_CFUNCTION(LFunctionLength, 1, 1, "#<FUNCTION LENGTH>", "LENGTH")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionLength::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(paramsv[0].IsEmptyList()) {
        lf.RegularReturn(0);
        return;
    }
    SExpressionCons *dp = paramsv[0].DynamicCastGetPtr<SExpressionCons>();
    INTELIB_ASSERT(dp, IntelibX_not_a_list(paramsv[0]));
    int len = 1;
    while((dp = dp->Cdr().DynamicCastGetPtr<SExpressionCons>()))
        len++;
    lf.RegularReturn(LReference(len));
}

#endif

