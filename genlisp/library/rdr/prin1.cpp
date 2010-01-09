//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/rdr/prin1.cpp
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

DECLARE_CFUNCTION(SFunctionPrin1,  1,  2,  "#<FUNCTION PRIN1>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionPrin1::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
    // <object> <stream>
{
    if(paramsc == 2) {
        SExpressionStream *stream = 
            paramsv[1].DynamicCastGetPtr<SExpressionStream>();
        INTELIB_ASSERT(stream, IntelibX_not_a_stream(paramsv[1]));
        stream->Puts(paramsv[0]->TextRepresentation().c_str());
    } else {
        SStreamStdout s;
        s->Puts(paramsv[0]->TextRepresentation().c_str());
    }
    lf.RegularReturn(paramsv[0]);
}

#endif

