//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/break.cpp
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

DECLARE_CFUNCTION(SchFunctionBreak, 0, 1, "#<FUNCTION BREAK>", "BREAK")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION

void (*TheSchemeBreakFunction)(const SReference &ref,
                               IntelibContinuation *lf) = 0;

void SchFunctionBreak::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(!TheSchemeBreakFunction) {
        lf.RegularReturn(*PTheSchemeBooleanFalse);
        return;
    }
    TheSchemeBreakFunction(paramsv[0], &lf);
    lf.RegularReturn(*PTheSchemeBooleanTrue);
}
#endif

