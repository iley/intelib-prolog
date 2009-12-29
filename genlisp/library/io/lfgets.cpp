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




#include "io_inc.h"

DECLARE_CFUNCTION(SFunctionLfgets,  1,  1,  "#<FUNCTION LFGETS>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionLfgets::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SExpressionStream *stream =
        paramsv[0].DynamicCastGetPtr<SExpressionStream>();
    INTELIB_ASSERT(stream, IntelibX_not_a_stream(paramsv[0]));
    SString ret;
    char buf[512];
    char *res;
    bool read_anything = false;
    bool incomplete_line;
    do {
        res = stream->Gets(buf, sizeof(buf));
        if(res) {
            ret += buf;
            read_anything = true;
            incomplete_line = true;
            for(unsigned int i = 0; i<sizeof(buf) && buf[i]; i++) {
                if(buf[i] == '\n') {
                    incomplete_line = false;
                    break;
                }
            }
        } else {
            incomplete_line = false;
        }
    } while (incomplete_line);
    if(!read_anything) {
        lf.RegularReturn(lf.False());
    } else {
        lf.RegularReturn(buf);
    }
}

#endif

