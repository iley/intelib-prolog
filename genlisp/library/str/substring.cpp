//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/str/substring.cpp
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

DECLARE_CFUNCTION(SFunctionSubstring, 3, 3, "#<FUNCTION SUBSTRING>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionSubstring::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{ // <string> <start> <end>     // conforms to R5RS
    const char *s = paramsv[0].GetString();
    int start = paramsv[1].GetInt();
    int end = paramsv[2].GetInt();
    if(start < 0 || end < start)
        throw IntelibX_invalid_arguments(SReference(start, end));
    int i;
    for(i = 0; i < start; i++) {
        if(!s[i]) {
            lf.RegularReturn("");
            return;
        }
    }
    char *buf = new char[end-start+1];
    for(i = start; i < end; i++)
        if(!(buf[i-start] = s[i])) break;
    buf[i-start] = 0;
    SExpressionString *resstr = new SExpressionString(buf);
    delete [] buf;
    lf.RegularReturn(SReference(resstr));
}
#endif

