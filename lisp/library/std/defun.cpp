//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/defun.cpp
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

DECLARE_SPECFORM(LFunctionDefun, "#<FORM DEFUN>", "DEFUN")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

#include "../../llambda.hpp"

void LFunctionDefun::
Call(const SReference &params, IntelibContinuation& lf) const
{
    const SReference *tmp = &params;
    LExpressionSymbol *symb = tmp->Car().DynamicCastGetPtr<LExpressionSymbol>();
    if(!symb) {
        throw IntelibX_lisp_not_a_symbol(tmp->Car());
    }
    tmp = &(tmp->Cdr());
    SReference *ll_p = &(tmp->Car());
    SReference *bd_p = &(tmp->Cdr());
    SReference fun(new LExpressionLambda(lf.GetContext(), *ll_p, *bd_p));
    symb->SetFunction(fun);
    lf.RegularReturn(symb);
}
#endif
