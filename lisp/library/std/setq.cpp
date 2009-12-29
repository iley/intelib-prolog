// +-------------------------------------------------------------------------+
// |               I__n__t__e__L__i__b           0.6.21 development          |
// | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
// |                                                                         |
// | This is free software. The library part is available under              |
// |                               GNU LESSER GENERAL PUBLIC LICENSE v.2.1.  |
// | GNU LGPL v2.1 is found in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
// |     Please see also docs/readme.txt and visit http://www.intelib.org    |
// |                                                                         |
// | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
// +-------------------------------------------------------------------------+




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
