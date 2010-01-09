//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/setq.cpp
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
