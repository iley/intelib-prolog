//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/io/lfgetc.cpp
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

DECLARE_CFUNCTION(SFunctionLfgetc,  1,  1,  "#<FUNCTION LFGETC>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionLfgetc::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SExpressionStream *stream =
        paramsv[0].DynamicCastGetPtr<SExpressionStream>();
    INTELIB_ASSERT(stream, IntelibX_not_a_stream(paramsv[0]));
    int got = stream->Getc();
    if(got == EOF) {
        lf.RegularReturn(lf.False());
    } else {
        unsigned char tmp[2] = "\0";
        tmp[0] = (unsigned char) got;
        lf.RegularReturn(SReference(tmp));
    }
}

#endif

