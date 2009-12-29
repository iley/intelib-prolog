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

