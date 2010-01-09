//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/listref.cpp
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

DECLARE_CFUNCTION(SchFunctionList_ref, 2,2,"#<FUNCTION LIST-REF>","LIST-REF")
#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION
void SchFunctionList_ref::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    const SReference *r = &(paramsv[0]);
    int idx = paramsv[1].GetInt();
    for(int i=0; i<idx; i++) r = &(r->Cdr());
    if(!r->IsEmptyList()) {
        lf.ReferenceReturn(r->Car(), *r);
    } else {
        lf.RegularReturn(*r);
    }
}
#endif

