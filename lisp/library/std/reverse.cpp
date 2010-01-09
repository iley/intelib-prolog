//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/reverse.cpp
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

DECLARE_CFUNCTION(LFunctionReverse, 1, 1, "#<FUNCTION REVERSE>", "REVERSE")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

static SReference do_reverse(const SReference &list,
                             const SReference &rest)
{
    if(list.IsEmptyList()) return rest;
    SExpressionCons *dp = list.SimpleCastGetPtr<SExpressionCons>();
    INTELIB_ASSERT(dp, IntelibX_not_a_list(list));
    return do_reverse(dp->Cdr(), SReference(dp->Car(), rest));
}

// <list>
void LFunctionReverse::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    lf.RegularReturn(do_reverse(paramsv[0], *PTheEmptyList));
}

#endif

