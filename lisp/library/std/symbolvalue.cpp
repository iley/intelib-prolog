//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/symbolvalue.cpp
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

DECLARE_CFUNCTION(LFunctionSymbolvalue, 1, 1,
                  "#<FUNCTION SYMBOLVALUE>", "SYMBOLVALUE")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionSymbolvalue::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    LExpressionSymbol *symb =
        paramsv[0].DynamicCastGetPtr<LExpressionSymbol>();
    INTELIB_ASSERT(symb, IntelibX_lisp_not_a_symbol(paramsv[0]));
    lf.ReferenceReturn(symb->GetDynamicValueRef(), paramsv[0]);
}

#endif

