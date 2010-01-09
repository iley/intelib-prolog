//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/rplaca.cpp
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

DECLARE_CFUNCTION(LFunctionRplaca, 2, 2, "#<FUNCTION RPLACA>", "RPLACA")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionRplaca::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SExpressionCons *dp =
        paramsv[0].DynamicCastGetPtr<SExpressionCons>();
    INTELIB_ASSERT(dp, IntelibX_not_a_list(paramsv[0]));
    dp->Car() = paramsv[1];
    lf.RegularReturn(paramsv[0]);
}

#endif

