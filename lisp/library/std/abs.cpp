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

DECLARE_CFUNCTION(LFunctionAbs, 1, 1, "#<FUNCTION ABS>", "ABS")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionAbs::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SExpressionInt *si = paramsv[0].DynamicCastGetPtr<SExpressionInt>();
    if(si) {
        int i = si->GetValue();
        lf.RegularReturn(SReference(new SExpressionInt(i<0 ? -i : i)));
        return;
    }
    SExpressionFloat *sf = paramsv[0].DynamicCastGetPtr<SExpressionFloat>();
    if(sf) {
        float f = sf->GetValue();
        lf.RegularReturn(SReference(new SExpressionFloat(f<0 ? -f : f)));
        return;
    }
    throw IntelibX_not_a_number(paramsv[0]);
}

#endif

