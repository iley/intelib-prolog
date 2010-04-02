#include "nillrepl.hpp"

#ifndef INTELIB_NILL_TRACE_SUPPORT
#define INTELIB_NILL_TRACE_SUPPORT 1
#endif

#if INTELIB_NILL_TRACE_SUPPORT == 1
#include "illtrace.hpp"
#endif



/////////////////////////////////////////////////////////

class LFunctionBody : public SExpressionFunction {
public:
    LFunctionBody() : SExpressionFunction(1, 1) {}
    ~LFunctionBody() {}

    virtual SString TextRepresentation() const 
    { return "#<FUNCTION BODY>"; } 

    virtual void DoApply(int paramsc, const SReference paramsv[], 
                         IntelibContinuation &lf) const
    {
        LExpressionLambda *f =
            paramsv[0].DynamicCastGetPtr<LExpressionLambda>();
        if(f) { 
            lf.ReferenceReturn(f->GetBody(), paramsv[0]);
            return;
        }
        LExpressionMacro *m =
            paramsv[0].DynamicCastGetPtr<LExpressionMacro>();
        if(m) { 
            lf.ReferenceReturn(m->GetBody(), paramsv[0]);
            return;
        }
        lf.RegularReturn(*PTheEmptyList);
    }            
};



bool IntelibLispLoop::ImportSymbol(const SReference& symb, 
                                   const char *name, 
                                   bool safe)
{
#error
}

void IntelibLispLoop::ImportLexicalSymbols(IntelibContinuation *lf)
{
    SReference lex_symbols =
        static_cast<LExpressionContext*>(lf->GetContext().GetPtr())
            ->GetAllSymbols();
    while(!lex_symbols.IsEmptyList()) {
        LSymbolRef symb(lex_symbols.Car());
        ImportSymbol(symb);
        lex_symbols = lex_symbols.Cdr();
    }
}
