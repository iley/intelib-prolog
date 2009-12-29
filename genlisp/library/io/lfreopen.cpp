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

