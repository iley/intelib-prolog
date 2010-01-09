//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/quotient.cpp
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

DECLARE_CFUNCTION(SchFunctionQuotient, 2, 2, "#<FUNCTION QUOTIENT>", 
                                              "QUOTIENT")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION
void SchFunctionQuotient::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    lf.RegularReturn(paramsv[0].GetInt()/paramsv[1].GetInt());
}

#endif

