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

DECLARE_CFUNCTION(LFunctionAppend, -1, -1, "#<FUNCTION APPEND>", "APPEND")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
static LReference do_append(SExpressionCons *dp, LReference &last)
{
    SExpressionCons *next_dp = dp->Cdr().DynamicCastGetPtr<SExpressionCons>();
    if(next_dp) {
        return LReference(new SExpressionCons(dp->Car(),
                      do_append(next_dp, last)));
    } else {
        return LReference(new SExpressionCons(dp->Car(), last));
    }
}
void LFunctionAppend::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(paramsc == 0) {
        lf.RegularReturn(*PTheEmptyList);
        return;
    }
    LReference res = paramsv[paramsc-1];
    for(int i = paramsc-2; i>=0; i--) {
        SExpressionCons *dp = paramsv[i].DynamicCastGetPtr<SExpressionCons>();
        if(!dp) {
            if(paramsv[i].IsEmptyList())
                continue;
            else
                throw IntelibX_not_a_list(paramsv[i]);
        }
        res = do_append(dp, res);
    }
    lf.RegularReturn(res);
}

#endif

