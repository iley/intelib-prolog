//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/list.cpp
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

DECLARE_CFUNCTION(LFunctionList, -1, -1, "#<FUNCTION LIST>", "LIST")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionList::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    LReference res = *PTheEmptyList;
    for(int i=paramsc-1; i>=0; i--)
        res = new SExpressionCons(paramsv[i], res);
    lf.RegularReturn(res);
}
#endif
