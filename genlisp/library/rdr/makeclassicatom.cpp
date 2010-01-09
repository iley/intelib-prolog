//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/rdr/makeclassicatom.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "rdr_inc.h"

DECLARE_CFUNCTION(SFunctionMakeclassicatom,1,1,"#<FUNCTION MAKECLASSICATOM>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionMakeclassicatom::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SExpressionString *the_string =
        paramsv[0].DynamicCastGetPtr<SExpressionString>();
    INTELIB_ASSERT(the_string, IntelibX_not_a_string(paramsv[0]));
    SReference ret(new SExpressionClassicAtom(the_string->GetValue()));
    lf.RegularReturn(ret);
}

#endif

