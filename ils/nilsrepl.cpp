//   InteLib                                    http://www.intelib.org
//   The file ils/nilsrepl.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2010
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.


#include "../scheme/scheme.hpp"
#include "../scheme/schpack.hpp"
#include "../scheme/schfun.hpp"
#include "../scheme/schsymb.hpp"
#include "../scheme/schreadr.hpp"
#include "nilsrepl.hpp"


// #ifndef INTELIB_NILL_TRACE_SUPPORT
// #define INTELIB_NILL_TRACE_SUPPORT 1
// #endif
// 
// #if INTELIB_NILL_TRACE_SUPPORT == 1
// #include "illtrace.hpp"
// #endif



/////////////////////////////////////////////////////////


class SchFunctionBody : public SExpressionFunction {
public:
    SchFunctionBody() : SExpressionFunction(1,1) {}
    ~SchFunctionBody() {}

    virtual SString TextRepresentation() const 
    { return "#<FUNCTION BODY>"; } 

    virtual void DoApply(int paramsc, const SReference paramsv[], 
                         IntelibContinuation &lf) const
    {
        SchExpressionLambda *f =
            paramsv[0].DynamicCastGetPtr<SchExpressionLambda>();
        if(!f) { 
            lf.RegularReturn(*PTheEmptyList);
            return;
        }
        lf.ReferenceReturn(f->GetBody(), paramsv[0]);
    }            
};




IntelibSchemeLoop::IntelibSchemeLoop(const SReference& a_package)
    : IntelibRepl(a_package)
{
    AddCommonSymbols();
    MakeFunctionalSymbol(new SchFunctionBody, "BODY", "%%%BODY");

#if INTELIB_NILL_TRACE_SUPPORT == 1
    MakeFunctionalSymbol(new LFunctionTrace, "TRACE", "%%%TRACE");
    MakeFunctionalSymbol(new LFunctionUntrace, "UNTRACE", "%%%UNTRACE");
#endif
}

bool IntelibSchemeLoop::ImportSymbol(const SReference& symb, 
                                   const char *name, 
                                   bool safe)
{
    return 
        static_cast<SchExpressionPackage*>(package.GetPtr())->
            Import(symb, name, safe);
}

void IntelibSchemeLoop::ImportLexicalSymbols(IntelibContinuation *lf)
{
    SReference lex_symbols =
        static_cast<SchExpressionContext*>(lf->GetContext().GetPtr())
            ->GetAllSymbols();
    while(!lex_symbols.IsEmptyList()) {
        SchSymbolRef symb(lex_symbols.Car());
        ImportSymbol(symb);
        lex_symbols = lex_symbols.Cdr();
    }
}

IntelibReader* IntelibSchemeLoop::MakeLocalReader() const
{
    return new SchemeReader;
}

IntelibContinuation* IntelibSchemeLoop::MakeLocalContinuation() const
{
    return new SchemeContinuation;
}

SString
IntelibSchemeLoop::SpecificTextRepresentation(const SReference &r) const
{
    return SchReference(r).TextRepresentation();
}

void IntelibSchemeLoop::MakeFunctionalSymbol(SReference func,
                                             const char *name,
                                             const char *second_name)
{
    SchSymbol s(name);
    s->SetGlobalValue(func);
    ImportSymbol(s);
    if(second_name)
        ImportSymbol(s, second_name);
}
