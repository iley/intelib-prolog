//   InteLib                                    http://www.intelib.org
//   The file lisp/library/rdr/makepackage.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "rdr_inc.h"

DECLARE_CFUNCTION(LFunctionMakepackage, 1, 1,
                  "#<FUNCTION MAKEPACKAGE>", 
                  "MAKEPACKAGE")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

void LFunctionMakepackage::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SExpressionString *the_string = 
        paramsv[0].DynamicCastGetPtr<SExpressionString>();
    INTELIB_ASSERT(the_string, IntelibX_not_a_string(paramsv[0]));
    SReference ret(new LExpressionPackage(/*the_string->GetValue()*/));
    lf.RegularReturn(ret);
}

#endif

