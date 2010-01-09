//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/funcall.cpp
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

DECLARE_CFUNCTION(LFunctionFuncall, 1, -1, "#<FUNCTION FUNCALL>", "FUNCALL")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

static void place_to_results(int count, int paramsc,
                             const SReference *paramsv,
                             IntelibContinuation& lf)
{
    if(count >= paramsc) return;
    SReference save = paramsv[paramsc - 1 - count];
    place_to_results(count+1, paramsc, paramsv, lf);
    lf.PushResult(save);
}


void LFunctionFuncall::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    // we can't just place paramsv[] elements to the results stack
    // because the paramsv vector IS the part of the stack!
    place_to_results(0, paramsc, paramsv, lf);
    lf.PushTodo(paramsc - 1);
}

#endif

