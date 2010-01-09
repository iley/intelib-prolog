//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/abs.cpp
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

DECLARE_CFUNCTION(LFunctionAbs, 1, 1, "#<FUNCTION ABS>", "ABS")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionAbs::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SExpressionInt *si = paramsv[0].DynamicCastGetPtr<SExpressionInt>();
    if(si) {
        int i = si->GetValue();
        lf.RegularReturn(SReference(new SExpressionInt(i<0 ? -i : i)));
        return;
    }
    SExpressionFloat *sf = paramsv[0].DynamicCastGetPtr<SExpressionFloat>();
    if(sf) {
        float f = sf->GetValue();
        lf.RegularReturn(SReference(new SExpressionFloat(f<0 ? -f : f)));
        return;
    }
    throw IntelibX_not_a_number(paramsv[0]);
}

#endif

