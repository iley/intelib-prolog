//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/equal.cpp
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

DECLARE_CFUNCTION(SchFunctionEqual, 2, 2, "#<FUNCTION EQUAL?>", "EQUAL?")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION
void SchFunctionEqual::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    lf.RegularReturn(SchReference(paramsv[0]).IsEqual(paramsv[1]) ?
                     *PTheSchemeBooleanTrue : *PTheSchemeBooleanFalse);
}
#endif

