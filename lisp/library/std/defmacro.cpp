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

DECLARE_SPECFORM(LFunctionDefmacro, "#<FUNCTION DEFMACRO>", "DEFMACRO")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

#include "../../llambda.hpp"

void LFunctionDefmacro::
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
    SReference fun(new LExpressionMacro(lf.GetContext(), *ll_p, *bd_p));
    symb->SetFunction(fun);
    lf.RegularReturn(symb);
}

#endif

