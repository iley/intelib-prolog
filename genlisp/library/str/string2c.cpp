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

