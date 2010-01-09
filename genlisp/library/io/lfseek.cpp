//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/io/lfseek.cpp
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

DECLARE_CFUNCTION(SFunctionLfseek,  2,  2,  "#<FUNCTION LFSEEK>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionLfseek::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
// (seek stream pos)
{
    SExpressionStream *stream =
        paramsv[0].DynamicCastGetPtr<SExpressionStream>();
    INTELIB_ASSERT(stream, IntelibX_not_a_stream(paramsv[0]));
    long pos = paramsv[1].GetInt();
    int res = stream->Seek(pos);
    if(res == -1) {
        lf.RegularReturn(lf.False());
    } else {
        lf.RegularReturn(0);
    }
}

#endif

