//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/let.cpp
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

DECLARE_SPECFORM(SchFunctionLet, "#<FORM LET>", "LET")
DECLARE_SPECFORM(SchFunctionLets, "#<FORM LET*>", "LET*")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION

#include "../../../sexpress/slocatn.hpp"

static void eval_bind_values(const SReference& list, IntelibContinuation& lf,
                             const SchContextRef &context)
{
    SExpressionCons *ls = list.DynamicCastGetPtr<SExpressionCons>();
    if(!ls) return; // we don't check for empty list, just ignore dotted rest
    eval_bind_values(ls->Cdr(), lf, context);
    SchExpressionSymbol *symb =
        ls->Car().Car().DynamicCastGetPtr<SchExpressionSymbol>();
    if(!symb) throw IntelibX_scheme_not_a_symbol(ls->Car().Car());
    SReference *location = context->ProvideBinding(symb);
    lf.PushTodo(IntelibContinuation::assign_to,
                SReference(new SExpressionLocation(context, location)));
    lf.PushTodo(IntelibContinuation::just_evaluate, ls->Car().Cdr().Car());
}

static void do_let(const SReference &paramsv, IntelibContinuation& lf,
                   bool inits_within_context)
{
        // the very last step is the context restoration
    lf.PushTodo(IntelibContinuation::set_context, lf.GetContext());
        // the next-to-last step is evaluation of the body
    lf.PushTodo(IntelibContinuation::evaluate_progn, paramsv.Cdr());

    SchContextRef tempcontext(new SchExpressionContext(lf.GetContext()));

    if(!inits_within_context)
        lf.PushTodo(IntelibContinuation::set_context, tempcontext);

        // the first step is to evaluate initial values    
        // and bind them within the context
    eval_bind_values(paramsv.Car(), lf, tempcontext);

    if(inits_within_context)
        lf.PushTodo(IntelibContinuation::set_context, tempcontext);
}

void SchFunctionLet::
Call(const SReference &paramsv, IntelibContinuation& lf) const
{
    do_let(paramsv, lf, false);
}

void SchFunctionLets::
Call(const SReference &paramsv, IntelibContinuation& lf) const
{
    do_let(paramsv, lf, true);
}

#endif
