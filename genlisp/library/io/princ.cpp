//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/io/princ.cpp
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

DECLARE_CFUNCTION(SFunctionPrinc,  1,  2,  "#<FUNCTION PRINC>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION



static int princ_do(const SReference s, SExpressionStream *stream);

static int princ_do_list(const SExpressionCons *dp, 
                         SExpressionStream *stream)
{
    int res;
    if(EOF==(res=princ_do(dp->Car(), stream))) return EOF;
    if(dp->Cdr().IsEmptyList()) return res;
    const SExpressionCons* dp2 = 
        dp->Cdr().DynamicCastGetPtr<SExpressionCons>();
    if(dp2) {
        if(EOF==stream->Puts(" ")) return EOF;
        return princ_do_list(dp2, stream);
    } else {
        if(EOF==stream->Puts(" . ")) return EOF;
        return princ_do(dp->Cdr(), stream);
    }
}

static int princ_do(const SReference s, SExpressionStream *stream)
{
    if(s->TermType() == SExpressionCons::TypeId) {
        if(EOF==stream->Puts("(")) return EOF;
        SExpressionCons *dp = static_cast<SExpressionCons*>(s.GetPtr()); 
        if(EOF==princ_do_list(dp, stream)) return EOF;
        return stream->Puts(")");
    } else {
        SExpressionString *tstr = 
            s.DynamicCastGetPtr<SExpressionString>();
        if(tstr)
            return stream->Puts(tstr->GetValue());

        SExpressionChar *tchr = 
            s.DynamicCastGetPtr<SExpressionChar>();
        if(tchr)
            return stream->Putc(tchr->GetValue());

        const char *ch;
#if INTELIB_TEXT_REPRESENTATIONS == 1
        SString tmpstr = s->TextRepresentation(); 
        ch = tmpstr.c_str(); 
#else
        h = "#<-\?\?\?->";
#endif
        return stream->Puts(ch);
   }
}

void SFunctionPrinc::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{   
    if(paramsc == 1) {
        SStreamStdout sto;
        princ_do(paramsv[0], sto.GetPtr());
    } else {
        SExpressionStream *stream = 
            paramsv[1].DynamicCastGetPtr<SExpressionStream>();
        INTELIB_ASSERT(stream, IntelibX_not_a_stream(paramsv[1]));
        princ_do(paramsv[0], stream);
    }

    lf.RegularReturn(paramsv[0]); 
}

#endif

