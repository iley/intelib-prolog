//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/str/string2list.cpp
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

DECLARE_CFUNCTION(SFunctionString2list, 1, 1, "#<FUNCTION STRING->LIST>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

#include <string.h>

void SFunctionString2list::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    int i;
    SReference res = *PTheEmptyList;
    const char *s = paramsv[0].GetString();
    for(i = strlen(s)-1; i >= 0; i--) {
        res = SReference(SReference((char)(s[i])), res);
    }
    lf.RegularReturn(res);
}

#endif

