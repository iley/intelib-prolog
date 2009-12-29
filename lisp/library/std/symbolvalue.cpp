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

