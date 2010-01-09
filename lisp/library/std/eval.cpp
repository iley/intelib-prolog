//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/eval.cpp
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

DECLARE_CFUNCTION(LFunctionEval, 1, 1, "#<FUNCTION EVAL>", "EVAL")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionEval::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    lf.TailReturn(paramsv[0]);
}

#endif

