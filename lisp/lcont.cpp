//   InteLib                                    http://www.intelib.org
//   The file lisp/lcont.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "../sexpress/iexcept.hpp"
#include "../sexpress/sstring.hpp"
#include "lisp.hpp"
#include "lcont.hpp"
#include "lsymbol.hpp"
#include "llambda.hpp"


////////////////////////////////////////////////////////

IntelibTypeId LExpressionContext::TypeId(&SExpression::TypeId, false);

LExpressionContext::LExpressionContext()
    : SExpression(TypeId)
{}

LExpressionContext::LExpressionContext(LContextRef a_prev)
    : SExpression(TypeId), prev(a_prev)
{}

LExpressionContext::~LExpressionContext()
{}

void LExpressionContext::AddBinding(const LExpressionSymbol *symb,
                                    const SReference& val)
{
    *valtbl.AddBinding(symb) = val;
}

SReference*
LExpressionContext::GetBinding(const LExpressionSymbol *symb) const
{
    SReference *res = valtbl.GetBinding(symb);
    if(res) return res;
    if(prev.GetPtr()) {
        return prev->GetBinding(symb);
    } else {
        return 0;
    }
}

SReference*
LExpressionContext::ProvideBinding(const LExpressionSymbol *symb)
{
    SReference *res = valtbl.GetBinding(symb);
    if(!res) res = valtbl.AddBinding(symb);
    return res;
}

void LExpressionContext::AddFunBinding(const LExpressionSymbol *symb,
                                       const SReference& val)
{
    *funtbl.AddBinding(symb) = val;
}

SReference*
LExpressionContext::GetFunBinding(const LExpressionSymbol *symb) const
{
    SReference *res = funtbl.GetBinding(symb);
    if(res) return res;
    if(prev.GetPtr()) {
        return prev->GetFunBinding(symb);
    } else {
        return 0;
    }
}

SReference*
LExpressionContext::ProvideFunBinding(const LExpressionSymbol *symb)
{
    SReference *res = funtbl.GetBinding(symb);
    if(!res) res = funtbl.AddBinding(symb);
    return res;
}


SReference LExpressionContext::GetAllSymbols() const
{
    SReference ret;
    if(prev.GetPtr()) {
        ret = prev->GetAllSymbols();
    } else {
        ret = *PTheEmptyList;
    }
    //
    {
        IntelibBindingsSet::Iterator iter(valtbl);
        const SExpressionLabel *key;
        SReference t;
        while(iter.GetNext(key, t)) {
            ret = SReference((LExpressionSymbol*)key, ret);
        }
    }
    //
    {
        IntelibBindingsSet::Iterator iter(funtbl);
        const SExpressionLabel *key;
        SReference t;
        while(iter.GetNext(key, t)) {
            ret = SReference((LExpressionSymbol*)key, ret);
        }
    }
    return ret;
}

SString LExpressionContext::TextRepresentation() const
{
    return "#<LISP-CONTEXT>";
}

/////////////////////////////////////////////////

void LispContinuation::CustomCommand(int opcode, const SReference& param)
{
    switch(opcode) {
        case take_result_as_form: {
            SReference r;
            PopResult(r);
            PushTodo(just_evaluate, r);
            break;
        }
        case duplicate_last_result: {
            SReference r;
            PopResult(r);
            PushResult(r);
            PushResult(r);
            break;
        }
        default:
                // this will just throw an exception
            IntelibContinuation::CustomCommand(opcode, param);
    }
}


#if 0
void LispContinuation::DoFunctionCall(const SReference &fun_ref,
                                     int paramscount,
                                     const SReference *paramsvector)
{
    LExpressionFunction *fun =
        fun_ref.DynamicCastGetPtr<LExpressionFunction>();
    if(!fun) {
        throw IntelibX_lisp_not_a_function(fun_ref);
    }
    fun->Apply(paramscount, paramsvector, *this);
}
#endif

#if 0
// not static because is used by LFunctionFunction
LReference RetrieveFunctionObject(LReference from, 
                                  const LContextRef &lc) 
{
    const LExpressionSymbol *symb =
        from.DynamicCastGetPtr<LExpressionSymbol>();
    if(symb) {
        const SReference *tmp = 0; 
        if(lc.GetPtr())
            tmp = lc->GetFunBinding(symb);
        if(!tmp || !tmp->GetPtr())
            tmp = &symb->GetFunction();
        if(tmp->GetPtr()) {
            return *tmp;
        }
        throw IntelibX_no_associated_function(from);
    }

    SExpressionCons *dp = 
        from.DynamicCastGetPtr<SExpressionCons>();
    if(dp) {
        if(dp->Car() == *PTheLispSymbolLambda) {
            LReference tmp = dp->Cdr(); 
            return LReference(new LExpressionLambda(lc, tmp.Car(), 
                                                    tmp.Cdr()));
        }
        throw IntelibX_no_associated_function(from);
    }      
    
    SExpressionFunction *fn = from.DynamicCastGetPtr<SExpressionFunction>();
    if(fn) return from;

    SExpressionForm *frm = from.DynamicCastGetPtr<SExpressionForm>();
    if(frm) return frm;

    throw IntelibX_no_associated_function(from);
}
#endif

void LispContinuation::EvaluateForm(const SExpressionCons *form)
{
    SExpressionSpecialToken *tk =
        form->Car().DynamicCastGetPtr<SExpressionSpecialToken>();
    if(tk) {
        SExpressionForm *f;
        SReference func;
        tk->GetApplicable(&f, &func, *this);
        if(f) { // special form
            f->Call(form->Cdr(), *this);
        } else {
            if(!func.GetPtr())
                throw IntelibX_no_associated_function(tk);
            PushResult(func);
//#error from might be null pointer... may throw but its not a bug!
            INTELIB_ASSERT(form, IntelibX_unexpected_unbound_value());
            SExpressionCons *cdr =
                form->Cdr().DynamicCastGetPtr<SExpressionCons>();
            if(cdr) {
                PlaceFormToStack(cdr, 1);
            } else {
                PushTodo(0);
            }
        }
    } else {
        // may be it's a lambda-list?
        SExpressionCons *dp = 
            form->Car().DynamicCastGetPtr<SExpressionCons>();
        if(dp) {
            if(dp->Car() == *PTheLispSymbolLambda) {
                LReference tmp = dp->Cdr(); 
                PushResult(new LExpressionLambda(GetContext(),
                                                 tmp.Car(), 
                                                 tmp.Cdr()));
                SExpressionCons *cdr =
                    form->Cdr().DynamicCastGetPtr<SExpressionCons>();
                if(cdr) {
                    PlaceFormToStack(cdr, 1);
                } else {
                    PushTodo(0);
                }
            } else {
                throw IntelibX_not_a_function(form->Car());
            }
        }
        // okay, the last chance is that it is just a function...
        // let's place it to the stack, may be it will work
        PlaceFormToStack(form, 0);
    }
}

IntelibX_lisp_not_a_function::
IntelibX_lisp_not_a_function(SReference a_param) 
    : IntelibX("lisp: not a function", a_param) {}

IntelibX_lisp_not_a_context::
IntelibX_lisp_not_a_context(SReference a_param) 
    : IntelibX("lisp: not a context", a_param) {}

IntelibX_lisp_symbol_has_no_value::
IntelibX_lisp_symbol_has_no_value(SReference a_param) 
    : IntelibX("lisp: symbol has no value", a_param) {}

IntelibX_no_associated_function::
IntelibX_no_associated_function(SReference a_param) 
    : IntelibX("No associated function", a_param) {}
