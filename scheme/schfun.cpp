//   InteLib                                    http://www.intelib.org
//   The file scheme/schfun.cpp
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
#include "schcont.hpp"
#include "schsymb.hpp"
#include "schfun.hpp"

IntelibTypeId SchExpressionLambda::TypeId(&SExpressionFunction::TypeId);

SchExpressionLambda::SchExpressionLambda(const SchContextRef &a_cx,
                                         const SReference &a_lambda,
                                         const SReference &a_body)
    : SExpressionFunction(TypeId, -1, -1),
      context(a_cx), lambda_list(a_lambda), body(a_body)
{
    DoAnalyseLambdaList(a_lambda, 0);
}

SchExpressionLambda::~SchExpressionLambda()
{
    delete[] lambda_vector;
}

void SchExpressionLambda::DoApply(int paramc, const SReference *paramv,
                                  IntelibContinuation &lf) const
{
    SchContextRef tempcontext(new SchExpressionContext(context));
    for(int i = 0; i<min_param; i++) {
        tempcontext->AddBinding(lambda_vector[i], paramv[i]);
    }
    if(lambda_rest) {
        SReference ls(*PTheEmptyList);
        for(int i=paramc-1; i>=min_param; i--)
            ls = SReference(paramv[i], ls);
        tempcontext->AddBinding(lambda_rest, ls);
    }

    lf.PushTodo(SchemeContinuation::set_context, lf.GetContext());
    lf.PushTodo(SchemeContinuation::evaluate_progn, body);
    lf.PushTodo(SchemeContinuation::set_context, tempcontext);
}

void SchExpressionLambda::DoAnalyseLambdaList(const SReference &rest, int n)
{
    SExpressionCons *dp = rest.DynamicCastGetPtr<SExpressionCons>();
    if(dp) {
        DoAnalyseLambdaList(dp->Cdr(), n+1);
        lambda_vector[n] =
            static_cast<SchExpressionSymbol*>(dp->Car().GetPtr());
        INTELIB_ASSERT(dp->Car().DynamicCastGetPtr<SchExpressionSymbol>(),
                       IntelibX_scheme_not_a_symbol(dp->Car()));
    } else {
        lambda_vector = new SchExpressionSymbol* [n];
        min_param = n;
        if(rest.IsEmptyList()) {
            max_param = n;
            lambda_rest = 0;
        } else {
            max_param = -1;
            lambda_rest = 
                static_cast<SchExpressionSymbol*>(rest.GetPtr());
            INTELIB_ASSERT(rest.DynamicCastGetPtr<SchExpressionSymbol>(),
                           IntelibX_scheme_not_a_symbol(dp->Car()));
        }
    }
}

#if 0
void SchExpressionLambda::DoPushBody(const SReference &rest,
                                     SchemeContinuation &cont)
{
    if(rest.IsEmptyList()) return;
    DoPushBody(rest.Cdr(), cont);
    cont.PushTodo(SchemeContinuation::just_evaluate, rest.Car());
}
#endif

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString SchExpressionLambda::TextRepresentation() const
{
    SString str("#<CLOSURE ");
    str += lambda_list->TextRepresentation();
    str += body->TextRepresentation();
    str += ">";
    return str;
}
#endif

