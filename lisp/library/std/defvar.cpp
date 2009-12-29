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

