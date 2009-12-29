// +-------------------------------------------------------------------------+
// |               I__n__t__e__L__i__b           0.6.21 development          |
// | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
// |                                                                         |
// | This is free software.  See GNU GENERAL PUBLIC LICENSE v.2 for details. |
// | GNU GPL v2  is  found  in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
// |     Please see also docs/readme.txt and visit http://www.intelib.org    |
// |                                                                         |
// | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
// +-------------------------------------------------------------------------+




// ///////////////////////////////////////////////////////////////////
// Trace support for NILL
// The initial version was contributed by Olga Shtefan a.k.a. Intafy
//     (http://www.intelib.org/cgi-bin/wiki/awki.cgi/IntAfy)
// Heavily modified.  The bugs are probably mine -- avst.
// ///////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "../sexpress/sexpress.hpp"
#include "../sexpress/sstring.hpp"
#include "../genlisp/conteval.hpp"
#include "../genlisp/lispform.hpp"
#include "../lisp/lisp.hpp"
#include "../lisp/lsymbol.hpp"
#include "../lisp/lcont.hpp"

#include "illtrace.hpp"

//////////////
// stub function

class LTraceStubFunction : public SExpressionFunction {
    friend class LTraceResultPrint;
    SString name;
    LReference oldbody;
    static int depth;
public:
    static IntelibTypeId TypeId;
    
    LTraceStubFunction(const char* n, LReference body);
    ~LTraceStubFunction() {}
    
    LReference& GetOldBody() {return oldbody; }
    
    virtual void DoApply(int paramsc, 
                         const SReference paramsv[],
                         IntelibContinuation &lf) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1    
    virtual SString TextRepresentation() const;
#endif
};

class LTraceResultPrint : public SExpressionFunction {
public:
    LTraceResultPrint() : SExpressionFunction(2, 2) {}
    ~LTraceResultPrint() {}
    
    virtual void DoApply(int paramsc, 
                         const SReference paramsv[],
                         IntelibContinuation &lf) const
    {
        int dep = --LTraceStubFunction::depth;
        for (int i=0; i < dep; i++) printf("  ");
        printf("(%s ...) -> %s\n", paramsv[0].GetString(), 
                       LReference(paramsv[1]).TextRepresentation().c_str());
        lf.RegularReturn(paramsv[1]);
    }

#if INTELIB_TEXT_REPRESENTATIONS == 1    
    virtual SString TextRepresentation() const { return "#<-RESULT_PRINT->"; }
#endif
};


IntelibTypeId
LTraceStubFunction::TypeId(&SExpressionFunction::TypeId);

int LTraceStubFunction::depth = 0;
        
LTraceStubFunction::LTraceStubFunction(const char* n, LReference body) 
    : SExpressionFunction(TypeId, -1, -1), name(n), oldbody(body) {}

void LTraceStubFunction::DoApply(int paramsc, 
                                 const SReference paramsv[],
                                 IntelibContinuation &lf) const
{
    // Printing arguments
    for (int i=0; i < depth; i++)
        printf("  ");
    depth++;
    printf("(%s", name.c_str());
    for (int i=0; i < paramsc; i++) {
        printf(" %s", 
               LReference(paramsv[i]).TextRepresentation().c_str());
    }
    printf(") -> ...\n");

    // We'll break the result stack scheduling various things to evaluate,
    // so we'll need to save the parameters before this happens
    SReference params = *PTheEmptyList;
    for (int i=paramsc; i>0; i--) {
        params = paramsv[i-1].MakeCons(params);
    }

    // Schedule printing of the results
    lf.PushResult(SReference(new LTraceResultPrint));
    lf.PushResult(name);
    lf.PushTodo(2);

    // Evaluating the substituted function
    lf.PushResult(oldbody); 
    while(!params.IsEmptyList()) {
        lf.PushResult(params.Car());
        params = params.Cdr();
    }
    lf.PushTodo(paramsc);
}

SString LTraceStubFunction::TextRepresentation() const
{
    return oldbody.TextRepresentation();
}


///////////////////////////////////////////////////



void LFunctionTrace::Call(const SReference &params,
                          IntelibContinuation &lf) const
{
    if (params.IsEmptyList()) {
        throw IntelibX_too_few_params(0);
    }
    if (!params.Cdr().IsEmptyList()) {
        throw IntelibX_too_many_params("2+");
    }
        
    LReference param(params.Car());
    
    LExpressionSymbol *symb =
        param.DynamicCastGetPtr<LExpressionSymbol>();

    if(symb) {
        const LReference *tmp = 0; 
        tmp = &symb->GetFunction();
        if(tmp->GetPtr()) {
            LTraceStubFunction *fun =
                tmp->DynamicCastGetPtr<LTraceStubFunction>();
            if (!fun) {
                SExpressionFunction *f = 
                    tmp->DynamicCastGetPtr<SExpressionFunction>();
                if(!f) throw IntelibX_not_a_function(*tmp);
                LTraceStubFunction * newfun = 
                    new LTraceStubFunction(symb->GetName(), f);
                symb->SetFunction(LReference(newfun));
                lf.RegularReturn(symb);
            } else {
                throw IntelibX("function already traced", param);
                //printf("This function is already traced.\n");
                //lf.RegularReturn(*PTheEmptyList);
            }
            return;
        }
        throw IntelibX_no_associated_function(symb);
    }
    throw IntelibX_lisp_not_a_symbol(param);
}


SString LFunctionTrace::TextRepresentation() const
{
    return SString("#<FUNCTION TRACE>");
}



void LFunctionUntrace::Call(const SReference &params,
                            IntelibContinuation &lf) const
{
    if (params.IsEmptyList()) {
        throw IntelibX_too_few_params(0);
    }
    if (!params.Cdr().IsEmptyList()) {
        throw IntelibX_too_many_params("2+");
    }
        
    LReference param(params.Car());
    
    LExpressionSymbol *symb =
        param.DynamicCastGetPtr<LExpressionSymbol>();

    if(symb) {
        const LReference *tmp = 0; 
        tmp = &symb->GetFunction();
        if(tmp->GetPtr()) {
            LTraceStubFunction *fun =
                tmp->DynamicCastGetPtr<LTraceStubFunction>();
            if (fun) {
                symb->SetFunction(fun->GetOldBody());
                lf.RegularReturn(symb);
            } else {
                throw IntelibX("function isn't traced", param);
                //printf("This function is not traced.\n");
                //lf.RegularReturn(*PTheEmptyList);
            }
            return;
        }
        throw IntelibX_no_associated_function(symb);
    }
    throw IntelibX_lisp_not_a_symbol(param);
}

SString LFunctionUntrace::TextRepresentation() const
{
    return SString("#<FUNCTION UNTRACE>");
}
