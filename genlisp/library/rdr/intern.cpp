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

