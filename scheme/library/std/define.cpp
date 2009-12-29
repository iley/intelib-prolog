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
