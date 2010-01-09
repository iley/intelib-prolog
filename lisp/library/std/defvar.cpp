//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/defvar.cpp
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

DECLARE_SPECFORM(LFunctionDefconstant, "#<FORM DEFCONSTANT>", "DEFCONSTANT")
DECLARE_SPECFORM(LFunctionDefvar, "#<FORM DEFVAR>", "DEFVAR")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

#include "../../../sexpress/slocatn.hpp"


static void defvarconst(const SReference &sympos, const SReference &valpos,
                        bool make_constant, IntelibContinuation& lf)
{
    LExpressionSymbol *symb = sympos.DynamicCastGetPtr<LExpressionSymbol>();
    if(!symb) {
        throw IntelibX_lisp_not_a_symbol(sympos);
    }

    // the returning value is the symbol
    lf.PushTodo(IntelibContinuation::quote_parameter, sympos);
    // assignment into the location
    lf.PushTodo(IntelibContinuation::assign_to,
                SReference(new SExpressionLocation(sympos,
                                            &(symb->GetDynamicValueRef()))));
    symb->SetDynamicBinding(true);
    if(make_constant) symb->SetConstant(true);

    lf.PushTodo(IntelibContinuation::just_evaluate, valpos);
}

void LFunctionDefconstant::
Call(const SReference &params, IntelibContinuation& lf) const
{
    defvarconst(params.Car(), params.Cdr().Car(), true, lf);
}

void LFunctionDefvar::
Call(const SReference &params, IntelibContinuation& lf) const
{
    defvarconst(params.Car(), params.Cdr().Car(), false, lf);
}

#endif

