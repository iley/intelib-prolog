//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/rem.cpp
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

DECLARE_CFUNCTION(LFunctionRem, 2, 2, "#<FUNCTION REM>", "REM")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionRem::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    intelib_integer_t op1 = paramsv[0].GetInt();
    intelib_integer_t op2 = paramsv[1].GetInt();

    /* the following magic is to imitate the CLISP behaviour */
    if(op2<0) op2 = -op2;
    intelib_integer_t res = op1>0 ? op1 % op2 : - (-op1) % op2;
    lf.RegularReturn(SReference(new SExpressionInt(res)));
}

#endif

