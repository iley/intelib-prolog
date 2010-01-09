//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/str/characterp.cpp
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

