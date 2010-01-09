//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/io/lftell.cpp
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

DECLARE_CFUNCTION(SFunctionLftell,  1,  1,  "#<FUNCTION LFTELL>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionLftell::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
// (tell stream)
{
    SExpressionStream *stream =
        paramsv[0].DynamicCastGetPtr<SExpressionStream>();
    INTELIB_ASSERT(stream, IntelibX_not_a_stream(paramsv[0]));
    long res = stream->Tell();
    if(res == -1) {
        lf.RegularReturn(lf.False());
    } else {
        lf.RegularReturn(res);
    }
}

#endif

