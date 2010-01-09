//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/str/list2string.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "str_inc.h"

DECLARE_CFUNCTION(SFunctionList2string, 1, 1, "#<FUNCTION LIST->STRING>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionList2string::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(paramsv[0].IsEmptyList()) {
        lf.RegularReturn("");
        return;
    }
    SString str;
    SExpressionCons *dp = paramsv[0].DynamicCastGetPtr<SExpressionCons>();
    INTELIB_ASSERT(dp, IntelibX_not_a_list(paramsv[0]));
    while(dp) {
        str += dp->Car().GetString();
        dp = dp->Cdr().DynamicCastGetPtr<SExpressionCons>();
    }
    lf.RegularReturn(str);
}

#endif

