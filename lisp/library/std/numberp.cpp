//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/numberp.cpp
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

DECLARE_CFUNCTION(LFunctionNumberp, 1, 1, "#<FUNCTION NUMBERP>", "NUMBERP")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionNumberp::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    const IntelibTypeId &tid = paramsv[0]->TermType();
    lf.RegularReturn(tid==SExpressionInt::TypeId||
                     tid==SExpressionFloat::TypeId ?
                     *PTheLispBooleanTrue : *PTheLispBooleanFalse);
}

#endif

