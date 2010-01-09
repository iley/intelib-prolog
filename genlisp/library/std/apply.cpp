//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/std/apply.cpp
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

DECLARE_CFUNCTION(SFunctionApply, 2, -1, "#<FUNCTION APPLY>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION
void SFunctionApply::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SReference argslist = paramsv[paramsc - 1];
    for(int i = paramsc-2; i>=1; i--) {
        argslist = SReference(paramsv[i], argslist);
    }
    lf.PushResult(paramsv[0]);
    int argscount = 0;
    while(!argslist.IsEmptyList()) {
        lf.PushResult(argslist.Car());
        argslist = argslist.Cdr();
        argscount++;
    }
    lf.PushTodo(argscount);
}
#endif
