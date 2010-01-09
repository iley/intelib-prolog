//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/str/string2c.cpp
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

DECLARE_CFUNCTION(SFunctionString2c, 1, 1, "#<FUNCTION STRING2C>")
DECLARE_CFUNCTION(SFunctionChar2c, 1, 1, "#<FUNCTION CHAR2C>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

#include <ctype.h>
#include <stdio.h>

static void next_char(SString &res, char c)
{
    switch(c) {
        case '\\': res += "\\\\"; break;
        case '\"': res += "\\\""; break;
        case '\n': res += "\\n"; break;
        case '\t': res += "\\t"; break;
        case '\r': res += "\\r"; break;
        default:
            if(isprint(c)) {
                char s[2];
                s[0] = c;
                s[1] = 0;
                res += s;
            } else {
                char s[10];
                sprintf(s, "\\%.3o", (int)(c));
                res += s;
            }
    }
}

void SFunctionString2c::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SString res = "\"";
    for(const char *p = paramsv[0].GetString(); *p; p++) {
        next_char(res, *p);
    }
    res += "\"";
    lf.RegularReturn(res);
}

void SFunctionChar2c::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    char c = paramsv[0].GetSingleChar();
    SString res = "\'";
    next_char(res, c);
    res += "\'";
    lf.RegularReturn(res);
}

#endif

