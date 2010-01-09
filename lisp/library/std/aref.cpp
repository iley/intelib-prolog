//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/aref.cpp
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

DECLARE_CFUNCTION(LFunctionAref, 2, 2, "#<FUNCTION AREF>", "AREF")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

#include "../../../sexpress/svector.hpp"

void LFunctionAref::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SExpressionVector *ev = paramsv[0].DynamicCastGetPtr<SExpressionVector>();
    INTELIB_ASSERT(ev, IntelibX_not_a_vector(paramsv[0]));
    SReference &ref = (*ev)[paramsv[1].GetInt()];
    if(!ref.GetPtr()) ref = *PTheEmptyList;
    lf.ReferenceReturn(ref, paramsv[0]);
}

#endif

