//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/error.cpp
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

