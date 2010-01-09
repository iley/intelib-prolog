//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/last.cpp
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

DECLARE_CFUNCTION(LFunctionLast, 1, 2, "#<FUNCTION LAST>", "LAST")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

static const SReference*
LFunctionLast_DoFindLast(const SReference &ref, int *remain)
{
    SExpressionCons *dp = ref.DynamicCastGetPtr<SExpressionCons>();
    const SReference *ret;
    if(dp) {
        ret = LFunctionLast_DoFindLast(dp->Cdr(), remain);
        if((*remain) > 0) {
            ret = &ref;
            (*remain)--;
        }
        return ret;
    } else {
        return &ref;
    }
}

void LFunctionLast::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(paramsv[0].IsEmptyList()) {
        lf.RegularReturn(paramsv[0]);
        return;
    }
    int taillen = 1;
    if(paramsc == 2)
        taillen = paramsv[1].GetInt();
    const SReference *retp = LFunctionLast_DoFindLast(paramsv[0], &taillen);
    if(retp == &(paramsv[0])) {
        lf.RegularReturn(*retp);
    } else {
        lf.ReferenceReturn(*const_cast<SReference*>(retp), paramsv[0]);
    }
}

#endif

