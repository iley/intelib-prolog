//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/io/lfreopen.cpp
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

DECLARE_CFUNCTION(SFunctionLfreopen,  1,  1,  "#<FUNCTION LFREOPEN>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionLfreopen::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SExpressionString *fname =
        paramsv[0].DynamicCastGetPtr<SExpressionString>();
    INTELIB_ASSERT(fname, IntelibX_not_a_string(paramsv[0]));
    SExpressionString *mode =
        paramsv[1].DynamicCastGetPtr<SExpressionString>();
    INTELIB_ASSERT(mode, IntelibX_not_a_string(paramsv[1]));
    SExpressionStreamFile *stream =
        paramsv[2].DynamicCastGetPtr<SExpressionStreamFile>();
    INTELIB_ASSERT(stream, IntelibX_not_a_stream(paramsv[2]));

    FILE *fl =
        freopen(fname->GetValue(), mode->GetValue(), stream->GetFile());

    if (!fl) {
        lf.RegularReturn(lf.False());
    } else {
        lf.RegularReturn(paramsv[2]);
    }
}

#endif

