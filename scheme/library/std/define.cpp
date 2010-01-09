//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/define.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "std_inc.h"

DECLARE_SPECFORM(SchFunctionDefine, "#<FORM DEFINE>", "DEFINE")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION
#include "../../../sexpress/slocatn.hpp"
void SchFunctionDefine::
Call(const SReference &paramsv, IntelibContinuation& lf) const
{
    const SReference *tmp = &paramsv;
    SchExpressionSymbol *symb =
        tmp->Car().DynamicCastGetPtr<SchExpressionSymbol>();
    if(symb) {
        // (define f value)
        // so, we must evaluate the value, and then assign 
        // finally, we should do 'unspecified return'
        lf.PushTodo(IntelibContinuation::return_unspecified);
        SReference loc;
        if(!lf.GetContext().GetPtr()) {
            // top level definition, see r5rs section 5.2 for explanation
            loc = SReference(
                new SExpressionLocation(tmp->Car(),
                                        &(symb->GetGlobalValue()))
            );
        } else {
            // internal definition
            SchContextRef c = lf.GetContext();
            loc = SReference(
                new SExpressionLocation(c, c->ProvideBinding(symb))
            );
        }
        lf.PushTodo(IntelibContinuation::assign_to, loc);
        lf.PushTodo(IntelibContinuation::just_evaluate, tmp->Cdr().Car());
    } else {
        // (define (f a b c) ...body...)
        // nothing to evaluate in this case
        // tmp = &(tmp->Cdr());
        symb = tmp->Car().Car().DynamicCastGetPtr<SchExpressionSymbol>();
        INTELIB_ASSERT(symb, IntelibX_scheme_not_a_symbol(tmp->Car()));
        SReference *ll_p = &(tmp->Car().Cdr());
        SReference *bd_p = &(tmp->Cdr());
        SReference fun(new SchExpressionLambda(lf.GetContext(), *ll_p, *bd_p));
        if(!lf.GetContext().GetPtr()) {
            symb->SetGlobalValue(fun);
        } else {
            // internal definition
            static_cast<SchemeContinuation&>(lf).GetContext()->
                AddBinding(symb, fun);
        }
        lf.ReturnUnspecified();
    }
}
#endif
