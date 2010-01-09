//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/io/lfgets.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




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

