//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/rreduce.cpp
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

DECLARE_CFUNCTION(LFunctionRreduce, 2, 3, "#<FUNCTION RREDUCE>", "RREDUCE")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION


// <fun> <list> [<initial>]
void LFunctionRreduce::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SReference fun = paramsv[0];
    SReference tmp = paramsv[1];
    SReference ini;
    if(paramsc > 2) ini = paramsv[2];
    while(!tmp.IsEmptyList()) {
        SReference &tmpcdr = tmp.Cdr();
        if(paramsc == 2 && tmpcdr.IsEmptyList()) {
            lf.PushResult(tmp.Car());
            break;
        }
        lf.PushResult(fun);
        lf.PushResult(tmp.Car());
        lf.PushTodo(2);
        tmp = tmpcdr;
    }
    if(paramsc > 2)
        lf.PushResult(ini);
}

#endif

