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

DECLARE_CFUNCTION(LFunctionError, -1, -1, "#<FUNCTION ERROR>", "ERROR")


#if defined(INTELIB_LISP_LIBRARY_HEADER_GENERATION) || \
    defined(INTELIB_LISP_LIBRARY_IMPLEMENTATION)

class IntelibX_lisp_error_function : public IntelibX {
public:
    IntelibX_lisp_error_function(SReference a_param);
};

#endif


#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

IntelibX_lisp_error_function::
IntelibX_lisp_error_function(SReference a_param)
        : IntelibX("Lisp ERROR function called", a_param) {}

void LFunctionError::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    LReference res = *PTheEmptyList;
    for(int i=paramsc-1; i>=0; i--)
        res = new SExpressionCons(paramsv[i], res);
    throw IntelibX_lisp_error_function(res);
}

#endif

