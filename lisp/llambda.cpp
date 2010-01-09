//   InteLib                                    http://www.intelib.org
//   The file lisp/llambda.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "../sexpress/sstring.hpp"
#include "../sexpress/slocatn.hpp"
#include "lcont.hpp"
#include "lsymbol.hpp"
#include "llambda.hpp"


LispLambdaBody::LispLambdaBody(const LContextRef &a_cx,
                               const SReference &a_lambda,
                               const SReference &a_body)
    : context(a_cx), lambda_list(a_lambda), body(a_body)
{
    DoAnalyseLambdaList(a_lambda, 0);
}

LispLambdaBody::~LispLambdaBody()
{
    delete[] lambda_vector;
}

void LispLambdaBody::DoAnalyseLambdaList(const SReference &rest, int n)
{
    SExpressionCons *dp = rest.DynamicCastGetPtr<SExpressionCons>();
    if(dp) {
        DoAnalyseLambdaList(dp->Cdr(), n+1);
        lambda_vector[n] =
            static_cast<LExpressionSymbol*>(dp->Car().GetPtr());
        INTELIB_ASSERT(dp->Car().DynamicCastGetPtr<LExpressionSymbol>(),
                       IntelibX_lisp_not_a_symbol(dp->Car()));
    } else {
        lambda_vector = new LExpressionSymbol* [n];
        lambda_length = n;
        if(rest.IsEmptyList()) {
            lambda_rest = 0;
        } else {
            lambda_rest = 
                static_cast<LExpressionSymbol*>(rest.GetPtr());
            INTELIB_ASSERT(rest.DynamicCastGetPtr<LExpressionSymbol>(),
                           IntelibX_lisp_not_a_symbol(dp->Car()));
        }
    }
}

////////////////////////////////////


IntelibTypeId LExpressionLambda::TypeId(&SExpressionFunction::TypeId);

LExpressionLambda::LExpressionLambda(const LContextRef &cx,
                                     const SReference &lambda,
                                     const SReference &body)
    : SExpressionFunction(TypeId, -1, -1), LispLambdaBody(cx, lambda, body)
{
    min_param = lambda_length;
    max_param = lambda_rest ? -1 : lambda_length;
}

LExpressionLambda::~LExpressionLambda()
{}

static void do_bind_symbol(LExpressionSymbol *symb,
                           const SReference &value,
                           LContextRef &context,
                           IntelibContinuation &lf)
{
    if(symb->IsDynamic()) {
        lf.PushTodo(
            IntelibContinuation::assign_to,
            new SExpressionLocation(symb, &(symb->GetDynamicValueRef()))
        );
        lf.PushTodo(
            IntelibContinuation::quote_parameter,
            symb->GetDynamicValue()
        );
        symb->SetDynamicValue(value);
    } else {
        context->AddBinding(symb, value);
    }
}

void LExpressionLambda::DoApply(int paramc, const SReference *paramv,
                                IntelibContinuation &lf) const
{
    LContextRef tempcontext(new LExpressionContext(context));
    for(int i = 0; i<min_param; i++) {
        //tempcontext->AddBinding(lambda_vector[i], paramv[i]);
        do_bind_symbol(lambda_vector[i], paramv[i], tempcontext, lf);
    }
    if(lambda_rest) {
        SReference ls(*PTheEmptyList);
        for(int i=paramc-1; i>=min_param; i--)
            ls = SReference(paramv[i], ls);
        //tempcontext->AddBinding(lambda_rest, ls);
        do_bind_symbol(lambda_rest, ls, tempcontext, lf);
    }

    lf.PushTodo(LispContinuation::set_context, lf.GetContext());
    lf.PushTodo(LispContinuation::evaluate_progn, body);
    lf.PushTodo(LispContinuation::set_context, tempcontext);
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString LExpressionLambda::TextRepresentation() const
{
    SString str("#<CLOSURE ");
    str += lambda_list->TextRepresentation();
    str += body->TextRepresentation();
    str += ">";
    return str;
}
#endif

////////////////////////////////////


IntelibTypeId LExpressionMacro::TypeId(&SExpressionForm::TypeId);

LExpressionMacro::LExpressionMacro(const LContextRef &cx,
                                     const SReference &lambda,
                                     const SReference &body)
    : SExpressionForm(TypeId), LispLambdaBody(cx, lambda, body)
{}

LExpressionMacro::~LExpressionMacro()
{}

void LExpressionMacro::Call(const SReference &params,
                            IntelibContinuation &lf) const
{
    LContextRef tempcontext(new LExpressionContext(context));
    const SReference *p = &params;
    for(int i = 0; i<lambda_length; i++) {
        //tempcontext->AddBinding(lambda_vector[i], p->Car());
        do_bind_symbol(lambda_vector[i], p->Car(), tempcontext, lf);
        p = &(p->Cdr());
    }
    if(lambda_rest) {
        //tempcontext->AddBinding(lambda_rest, *p);
        do_bind_symbol(lambda_rest, *p, tempcontext, lf);
    } else {
        if(!p->IsEmptyList()) {
            throw IntelibX_too_many_params(*p);
        }
    }

    lf.PushTodo(LispContinuation::take_result_as_form);
    lf.PushTodo(LispContinuation::set_context, lf.GetContext());
    lf.PushTodo(LispContinuation::evaluate_progn, body);
    lf.PushTodo(LispContinuation::set_context, tempcontext);
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString LExpressionMacro::TextRepresentation() const
{
    SString str("#<MACRO ");
    str += lambda_list->TextRepresentation();
    str += body->TextRepresentation();
    str += ">";
    return str;
}
#endif

////////////////////////////////////

