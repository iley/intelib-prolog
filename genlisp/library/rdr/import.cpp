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

