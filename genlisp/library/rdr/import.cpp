//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/rdr/import.cpp
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

DECLARE_CFUNCTION(SFunctionImport,  2,  2,  "#<FUNCTION IMPORT>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

static void do_import(SExpressionHashPackage *pkg,
                      const SReference &a,
                      const SReference &d)
{
    SReference la(a);
    if(!pkg->Import(la)) {
        throw IntelibX_package_conflict(a);
    }
    if(d.IsEmptyList()) return;
    SExpressionCons *dp = d.DynamicCastGetPtr<SExpressionCons>();
    INTELIB_ASSERT(dp, IntelibX_not_a_list(d));
    do_import(pkg, dp->Car(), dp->Cdr());
}


void SFunctionImport::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
       // <symbol(s)> <package>
{
    SExpressionHashPackage *pkg = 
        paramsv[1].DynamicCastGetPtr<SExpressionHashPackage>();
    INTELIB_ASSERT(pkg, IntelibX_not_a_package(paramsv[1]));

    SExpressionCons *dp = paramsv[0].DynamicCastGetPtr<SExpressionCons>();
    if(dp) {
        do_import(pkg, dp->Car(), dp->Cdr());
    } else {
        do_import(pkg, paramsv[0], *PTheEmptyList);
    }
    lf.RegularReturn(lf.True()); 
}

#endif

