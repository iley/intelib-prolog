//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/str/string2num.cpp
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

DECLARE_CFUNCTION(SFunctionString2num, 1, 1, "#<FUNCTION STRING->NUMBER>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

extern SReference Charp2LispNumber(const char* s);

void SFunctionString2num::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SExpressionString *tstr =
        paramsv[0].DynamicCastGetPtr<SExpressionString>();
    INTELIB_ASSERT(tstr, IntelibX_not_a_string(paramsv[0]));
    lf.RegularReturn(Charp2LispNumber(tstr->GetValue()));
}

#endif

