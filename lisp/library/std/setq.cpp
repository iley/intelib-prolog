//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/setq.cpp
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

DECLARE_SPECFORM(LFunctionSetq, "#<FORM SETQ>", "SETQ")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

#include "../../../sexpress/slocatn.hpp"

void LFunctionSetq::
Call(const SReference &params, IntelibContinuation& lf) const
{
    LReference lastresult = *PTheEmptyList;
    SReference curform = params;
    while(!curform.IsEmptyList()) {
        SReference symbref = curform.Car();
        SReference to_eval = curform.Cdr().Car();

        LExpressionSymbol *symb =
            symbref.DynamicCastGetPtr<LExpressionSymbol>();
        INTELIB_ASSERT(symb, IntelibX_lisp_not_a_symbol(curform.Car()));

        curform = curform.Cdr().Cdr();

        SReference loc;
        SReference *lex = 0;
        LContextRef c = lf.GetContext();
        if(c.GetPtr() && (lex = c->GetBinding(symb))) {
            loc = SReference(new SExpressionLocation(c, lex));
        } else {
            loc = SReference(
                new SExpressionLocation(symbref,
                                        &(symb->GetDynamicValueRef()))
            );
        }
        lf.PushTodo(IntelibContinuation::assign_to, loc);
        if(curform.IsEmptyList()) // the last one is to be returned
            lf.PushTodo(LispContinuation::duplicate_last_result);
        lf.PushTodo(IntelibContinuation::just_evaluate, to_eval);
    }
}
#endif
