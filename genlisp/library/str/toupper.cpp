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

DECLARE_CFUNCTION(SFunctionToupper,  1,  1,  "#<FUNCTION TOUPPER>")
DECLARE_CFUNCTION(SFunctionTolower,  1,  1,  "#<FUNCTION TOLOWER>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

#include <ctype.h>

static
void trans(const SReference &par, IntelibContinuation& lf, int (*func)(int))
{
    SExpressionChar *ch = par.DynamicCastGetPtr<SExpressionChar>();
    if(ch) {
        char res = func(ch->GetValue());
        lf.RegularReturn(res);
        return;
    }
    SExpressionString *tstr = par.DynamicCastGetPtr<SExpressionString>();
    INTELIB_ASSERT(tstr, IntelibX_not_a_string(par));
    SString res;
    const char *s = tstr->GetValue();
    while(*s) {
        res += (char) func(*s);
        s++;
    }
    lf.RegularReturn(res);
}


void SFunctionToupper::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    trans(paramsv[0], lf, toupper);
}
void SFunctionTolower::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    trans(paramsv[0], lf, tolower);
}

#endif

