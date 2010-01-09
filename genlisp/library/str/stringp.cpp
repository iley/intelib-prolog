//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/str/stringp.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "str_inc.h"

DECLARE_CFUNCTION(SFunctionStringp,  1,  1,  "#<FUNCTION STRINGP>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionStringp::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    lf.ReturnBoolean(paramsv[0]->TermType() == SExpressionString::TypeId);
}

#endif

