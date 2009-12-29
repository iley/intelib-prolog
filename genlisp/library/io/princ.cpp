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

