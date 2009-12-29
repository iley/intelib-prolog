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

DECLARE_SPECFORM(SchFunctionSetq, "#<FORM SET!>", "SET!")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION
#include "../../../sexpress/slocatn.hpp"
void SchFunctionSetq::
Call(const SReference &paramsv, IntelibContinuation& lf) const
{
    const SReference *tmp = &paramsv;
    SchExpressionSymbol *symb =
        tmp->Car().DynamicCastGetPtr<SchExpressionSymbol>();
    if(!symb) throw IntelibX_scheme_not_a_symbol(tmp->Car());

    // we must evaluate the value, and then assign 
    // finally, we should do 'unspecified return'
    lf.PushTodo(IntelibContinuation::return_unspecified);
    SReference loc;
    SReference *lex = 0;
    SchContextRef c = lf.GetContext();
    if(c.GetPtr() && (lex = c->GetBinding(symb))) {
        loc = SReference(new SExpressionLocation(c, lex));
    } else {
        loc = SReference(
            new SExpressionLocation(tmp->Car(),
                                    &(symb->GetGlobalValue()))
        );
    }
    lf.PushTodo(IntelibContinuation::assign_to, loc);
    lf.PushTodo(IntelibContinuation::just_evaluate, tmp->Cdr().Car());
}
#endif
