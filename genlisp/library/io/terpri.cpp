//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/io/terpri.cpp
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

DECLARE_CFUNCTION(SFunctionTerpri,  0,  1,  "#<FUNCTION TERPRI>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionTerpri::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{   
    if(paramsc == 1) {
        SExpressionStream *stream = 
            paramsv[0].DynamicCastGetPtr<SExpressionStream>();
        INTELIB_ASSERT(stream, IntelibX_not_a_stream(paramsv[0]));
        stream->Puts("\n");
    } else {
        SStreamStdout s;
        s->Puts("\n");
    }
    lf.RegularReturn(*PTheEmptyList);
}

#endif

