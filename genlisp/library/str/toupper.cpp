//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/str/toupper.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




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

