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




#include "str_inc.h"

DECLARE_CFUNCTION(SFunctionCharacterp, 1, 1, "#<FUNCTION CHARACTERP>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionCharacterp::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    bool res;
    if(paramsv[0]->TermType() == SExpressionChar::TypeId) {
        res = true;
#if 0 // No, it is no good to have (characterp "a") returning true!
    } else 
    if(paramsv[0]->TermType() == SExpressionString::TypeId) {
        const char *s =
            static_cast<SExpressionString*>(paramsv[0].GetPtr())->GetValue();
        res = s[0] && !s[1];  // string of exactly one char
#endif
    } else {
        res = false;
    }

    lf.ReturnBoolean(res);
}

#endif

