//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/rdr/readfromstring.cpp
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

