//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/io/lfungetc.cpp
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

DECLARE_CFUNCTION(SFunctionLfungetc,  2,  2,  "#<FUNCTION LFUNGETC>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionLfungetc::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
// (ungetc "a" file)
{
    SExpressionStream *stream =
        paramsv[1].DynamicCastGetPtr<SExpressionStream>();
    INTELIB_ASSERT(stream, IntelibX_not_a_stream(paramsv[1]));
    const char *ch = paramsv[0].GetString();
    INTELIB_ASSERT(ch[0] && ch[1]==0, IntelibX_not_a_char(paramsv[0]));
    int res = stream->Ungetc(ch[0]);
    if(res == EOF) {
        lf.RegularReturn(lf.False());
    } else {
        lf.RegularReturn(paramsv[0]);
    }
}

#endif

