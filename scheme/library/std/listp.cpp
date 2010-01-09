//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/listp.cpp
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

DECLARE_CFUNCTION(SchFunctionListp, 1, 1, "#<FUNCTION LIST?>", "LIST?")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION
void SchFunctionListp::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(paramsv[0].IsEmptyList()) {
        lf.RegularReturn(*PTheSchemeBooleanTrue);
        return;
    }
    SExpressionCons *cs = paramsv[0].DynamicCastGetPtr<SExpressionCons>();
    lf.RegularReturn(
        (cs &&
            (cs->Cdr().IsEmptyList() ||
            cs->Cdr().DynamicCastGetPtr<SExpressionCons>()))
                ? *PTheSchemeBooleanTrue : *PTheSchemeBooleanFalse
    );
}
#endif
