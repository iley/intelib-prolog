//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/consp.cpp
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

DECLARE_CFUNCTION(LFunctionConsp, 1, 1, "#<FUNCTION CONSP>", "CONSP")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionConsp::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    lf.RegularReturn(
        paramsv[0]->TermType().IsSubtypeOf(SExpressionCons::TypeId) ?
        *PTheLispBooleanTrue : *PTheLispBooleanFalse);
}

#endif

