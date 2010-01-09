//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/rdr/print.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "rdr_inc.h"

DECLARE_CFUNCTION(SFunctionPrint,  1,  2,  "#<FUNCTION PRINT>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionPrint::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
    // <object> <stream>
{
    if(paramsc == 2) {
        SExpressionStream *stream = 
            paramsv[1].DynamicCastGetPtr<SExpressionStream>();
        INTELIB_ASSERT(stream, IntelibX_not_a_stream(paramsv[1]));
        stream->Puts("\n");
        stream->Puts(paramsv[0]->TextRepresentation().c_str());
        stream->Putc(' ');
    } else {
        SStreamStdout s;
        s->Puts("\n");
        s->Puts(paramsv[0]->TextRepresentation().c_str());
        s->Putc(' ');
    }
    lf.RegularReturn(paramsv[0]);
}

#endif

