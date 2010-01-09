//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/rdr/read.cpp
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

DECLARE_CFUNCTION(SFunctionRead,  0,  3,  "#<FUNCTION READ>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionRead::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
       // <stream> [<eof-error-p> [<eof-value>]]
{
    SListConstructor L;
    SReference res;
    SStreamRef strm;
    if(paramsc > 0) {
        SExpressionStream *stream = 
            paramsv[0].DynamicCastGetPtr<SExpressionStream>();
        INTELIB_ASSERT(stream, IntelibX_not_a_stream(paramsv[0]));
        strm = stream;
    } else {
        strm = SStreamStdin();
    }
    if(PTheIntelibReader) {
        res = PTheIntelibReader->Read(strm);
    } else {
        IntelibReader reader;
        res = reader.Read(strm);
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

