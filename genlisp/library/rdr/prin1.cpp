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

