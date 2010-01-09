//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/eq.cpp
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

DECLARE_CFUNCTION(SchFunctionEq, 2, 2, "#<FUNCTION EQ?>", "EQ?")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION
void SchFunctionEq::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    lf.RegularReturn(paramsv[0] == paramsv[1] ?
                     *PTheSchemeBooleanTrue : *PTheSchemeBooleanFalse);
}

#endif

