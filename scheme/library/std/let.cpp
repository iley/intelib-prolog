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
