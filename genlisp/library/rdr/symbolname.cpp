//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/rdr/symbolname.cpp
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

DECLARE_CFUNCTION(SFunctionSymbolname, 1, 1, "#<FUNCTION SYMBOLNAME>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionSymbolname::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SExpressionLabel *lb =
        paramsv[0].DynamicCastGetPtr<SExpressionLabel>();
    INTELIB_ASSERT(lb, IntelibX_not_a_label(paramsv[0]));
    lf.RegularReturn(SReference(new SExpressionString(lb->GetName())));
}

#endif

