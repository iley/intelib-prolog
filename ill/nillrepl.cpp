//   InteLib                                    http://www.intelib.org
//   The file ill/nillrepl.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2010
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.


#include "../lisp/lisp.hpp"
#include "../lisp/lpackage.hpp"
#include "../lisp/llambda.hpp"
#include "../lisp/lsymbol.hpp"
#include "../lisp/lreader.hpp"
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

IntelibLispLoop::IntelibLispLoop(const SReference& a_package)
    : IntelibRepl(a_package)
{
    AddCommonSymbols();
    MakeFunctionalSymbol(new LFunctionBody, "BODY", "%%%BODY");

#if INTELIB_NILL_TRACE_SUPPORT == 1
    MakeFunctionalSymbol(new LFunctionTrace, "TRACE", "%%%TRACE");
    MakeFunctionalSymbol(new LFunctionUntrace, "UNTRACE", "%%%UNTRACE");
#endif
}

bool IntelibLispLoop::ImportSymbol(const SReference& symb, 
                                   const char *name, 
                                   bool safe)
{
    return 
        static_cast<LExpressionPackage*>(package.GetPtr())->
            Import(symb, name, safe);
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

IntelibReader* IntelibLispLoop::MakeLocalReader() const
{
    return new LispReader;
}

IntelibContinuation* IntelibLispLoop::MakeLocalContinuation() const
{
    return new LispContinuation;
}

SString
IntelibLispLoop::SpecificTextRepresentation(const SReference &r) const
{
    return LReference(r).TextRepresentation();
}

void IntelibLispLoop::MakeFunctionalSymbol(SReference func,
                                           const char *name,
                                           const char *second_name)
{
    LSymbol s(name);
    s->SetFunction(func);
    ImportSymbol(s);
    if(second_name)
        ImportSymbol(s, second_name);
}
