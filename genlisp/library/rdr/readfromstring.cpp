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

DECLARE_CFUNCTION(SFunctionReadfromstring, 1, 3, "#<FUNCTION READFROMSTRING>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionReadfromstring::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
    // <string> [<error-value> [<eof-value>]]
{
    SListConstructor L;
    SExpressionString *the_string =
        paramsv[0].DynamicCastGetPtr<SExpressionString>();
    INTELIB_ASSERT(the_string, IntelibX_not_a_string(paramsv[0]));
    SStreamCharbuf str(the_string->GetValue());
    SReference res;
    if(PTheIntelibReader) {
        res = PTheIntelibReader->Read(str);
    } else {
        IntelibReader reader;
        res = reader.Read(str);
    }
    SReference ret;
    if(res.GetPtr() == IntelibReader::EofMarker.GetPtr()) {
        /* EOF */
        // by default, <eof-error-p> is assumed true
        if(paramsc<2 || paramsv[1].GetPtr() == lf.False().GetPtr()) {
            throw IntelibX_read_eof(paramsv[0]);
        }
        // well, don't consider it error
        ret = paramsc >= 3 ? paramsv[2] : res /*LispReader::EofMarker*/;
    } else {
        ret = res;
    }
    lf.RegularReturn(ret);
}

#endif

