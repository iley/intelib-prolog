//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/break.cpp
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

DECLARE_CFUNCTION(LFunctionBreak, 0, 1, "#<FUNCTION BREAK>", "BREAK")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

void (*TheLispBreakFunction)(const SReference &ref,
                             IntelibContinuation *lf) = 0;

void LFunctionBreak::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(!TheLispBreakFunction) {
        lf.RegularReturn(*PTheLispBooleanFalse);
        return;
    }
    TheLispBreakFunction(paramsv[0], &lf);
    lf.RegularReturn(*PTheLispBooleanTrue);
}
#endif

