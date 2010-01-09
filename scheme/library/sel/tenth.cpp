//   InteLib                                    http://www.intelib.org
//   The file scheme/library/sel/tenth.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "sel_inc.h"

DECLARE_CFUNCTION(SchFunctionTenth, 1, 1, "#<FUNCTION TENTH>", "TENTH")
#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION
void SchFunctionTenth::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SReference *r = &(paramsv[0].Cdr().Cdr().Cdr().Cdr().Cdr().Cdr().Cdr().Cdr().Cdr().Car());
    if(r != PTheEmptyList) {
        lf.ReferenceReturn(*r, paramsv[0]);
    } else {
        lf.RegularReturn(*r);
    }
}
#endif

