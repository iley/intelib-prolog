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

DECLARE_CFUNCTION(SFunctionString2num, 1, 1, "#<FUNCTION STRING->NUMBER>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

extern SReference Charp2LispNumber(const char* s);

void SFunctionString2num::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SExpressionString *tstr =
        paramsv[0].DynamicCastGetPtr<SExpressionString>();
    INTELIB_ASSERT(tstr, IntelibX_not_a_string(paramsv[0]));
    lf.RegularReturn(Charp2LispNumber(tstr->GetValue()));
}

#endif
