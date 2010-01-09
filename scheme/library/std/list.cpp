//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/list.cpp
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

DECLARE_CFUNCTION(SchFunctionList, -1, -1, "#<FUNCTION LIST>", "LIST")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION
void SchFunctionList::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SReference res(*PTheEmptyList);
    for(int i = paramsc-1; i>=0; i--) res = SReference(paramsv[i], res); 
    lf.RegularReturn(res);
}
#endif
