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

DECLARE_CFUNCTION(LFunctionDifference, 1, -1, "#<FUNCTION DIFFERENCE>", "-")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionDifference::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(paramsc == 1) {
        // special case - unary minus
        if(paramsv[0]->TermType() == SExpressionInt::TypeId) {
            lf.RegularReturn(
                LReference(new SExpressionInt(-paramsv[0].GetInt())));
        } else
            if(paramsv[0]->TermType() == SExpressionFloat::TypeId) {
                lf.RegularReturn(
                    LReference(new SExpressionFloat(-paramsv[0].GetFloat())));
            }
        return;
    }
    // Ok, no more special cases
    LReference res = paramsv[0];
    for(int i = 1; i < paramsc; i++) {
        LReference op = paramsv[i];
        if(LispUnifyNumbericTypes(res, op) == SExpressionInt::TypeId) {
            res = new SExpressionInt(res.GetInt() - paramsv[i].GetInt());
        } else {
            res = new SExpressionFloat(res.GetFloat() - paramsv[i].GetFloat());
        }
    }
    lf.RegularReturn(res);
}

#endif

