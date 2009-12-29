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

DECLARE_CFUNCTION(LFunctionLast, 1, 2, "#<FUNCTION LAST>", "LAST")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

static const SReference*
LFunctionLast_DoFindLast(const SReference &ref, int *remain)
{
    SExpressionCons *dp = ref.DynamicCastGetPtr<SExpressionCons>();
    const SReference *ret;
    if(dp) {
        ret = LFunctionLast_DoFindLast(dp->Cdr(), remain);
        if((*remain) > 0) {
            ret = &ref;
            (*remain)--;
        }
        return ret;
    } else {
        return &ref;
    }
}

void LFunctionLast::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(paramsv[0].IsEmptyList()) {
        lf.RegularReturn(paramsv[0]);
        return;
    }
    int taillen = 1;
    if(paramsc == 2)
        taillen = paramsv[1].GetInt();
    const SReference *retp = LFunctionLast_DoFindLast(paramsv[0], &taillen);
    if(retp == &(paramsv[0])) {
        lf.RegularReturn(*retp);
    } else {
        lf.ReferenceReturn(*const_cast<SReference*>(retp), paramsv[0]);
    }
}

#endif

