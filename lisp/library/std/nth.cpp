//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/nth.cpp
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

DECLARE_CFUNCTION(LFunctionNth, 2, 2, "#<FUNCTION NTH>", "NTH")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionNth::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    int idx = paramsv[0].GetInt();
    SReference r = paramsv[1];
    for(int i=0; i<idx; i++) r = r.Cdr();
    if(!r.IsEmptyList()) {
        lf.ReferenceReturn(r.Car(), paramsv[1]);
    } else {
        lf.RegularReturn(r);
    }
}
#endif

