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

DECLARE_CFUNCTION(SchFunctionAppend, -1, -1, "#<FUNCTION APPEND>", "APPEND")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION
static SReference CopyListAndReplaceLast(SExpressionCons *dp,
                                         SReference &last)
{
    SExpressionCons *next_dp = dp->Cdr().DynamicCastGetPtr<SExpressionCons>();
    if(next_dp) {
        return SReference(dp->Car(), CopyListAndReplaceLast(next_dp, last));
    } else {
        return SReference(dp->Car(), last);
    }
}

void SchFunctionAppend::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(paramsc == 0) {
        lf.RegularReturn(*PTheEmptyList);
        return;
    }
    SchReference res = paramsv[paramsc-1];
    for(int i = paramsc-2; i>=0; i--) {
        SExpressionCons *dp = paramsv[i].DynamicCastGetPtr<SExpressionCons>();
        if(!dp) {
            if(paramsv[i].IsEmptyList())
                continue;
            else
                throw IntelibX_not_a_list(paramsv[i]);
        }
        res = CopyListAndReplaceLast(dp, res);
    }
    lf.RegularReturn(res);
}

#endif

