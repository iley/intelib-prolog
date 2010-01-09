//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/rdr/intern.cpp
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

DECLARE_CFUNCTION(SFunctionIntern,  2,  2,  "#<FUNCTION INTERN>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionIntern::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
       // <name> <package>
{
    SExpressionString *name = 
        paramsv[0].DynamicCastGetPtr<SExpressionString>();
    INTELIB_ASSERT(name, IntelibX_not_a_string(paramsv[0]));
    SExpressionHashPackage *pkg = 
        paramsv[1].DynamicCastGetPtr<SExpressionHashPackage>();
    INTELIB_ASSERT(pkg, IntelibX_not_a_package(paramsv[1]));
    lf.RegularReturn(pkg->Intern(name->GetValue())); 
}

#endif

