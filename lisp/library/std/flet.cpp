//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/flet.cpp
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

DECLARE_SPECFORM(LFunctionFlet, "#<FORM FLET>", "FLET")
DECLARE_SPECFORM(LFunctionLabels, "#<FORM LABELS>", "LABELS")
DECLARE_SPECFORM(LFunctionMacrolet, "#<FORM MACROLET>", "MACROLET")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

#include "../../lcont.hpp"
#include "../../llambda.hpp"

enum do_bindings_mode { bindings_flet, bindings_labels, bindings_macrolet };

static void do_bindings(const SReference &list,
                        const LContextRef &inner,
                        const LContextRef &outer,
                        do_bindings_mode mode)
{
    if(list.IsEmptyList()) return;
    SExpressionCons *dp =
        list.DynamicCastGetPtr<SExpressionCons>();
    INTELIB_ASSERT(dp, IntelibX_not_a_list(list));
    // dp->Car() must be a list
    SExpressionCons *bdp =
        dp->Car().DynamicCastGetPtr<SExpressionCons>();
    INTELIB_ASSERT(bdp, IntelibX_not_a_list(dp->Car()));
    LExpressionSymbol *symb =
        bdp->Car().DynamicCastGetPtr<LExpressionSymbol>();
    INTELIB_ASSERT(symb, IntelibX_lisp_not_a_symbol(bdp->Car()));

    LReference fun;
    switch(mode) {
        case bindings_flet:
            fun = new LExpressionLambda(outer,
                                        bdp->Cdr().Car(),
                                        bdp->Cdr().Cdr());
            break;
        case bindings_labels:
            fun = new LExpressionLambda(inner,
                                        bdp->Cdr().Car(),
                                        bdp->Cdr().Cdr());
            break;
        case bindings_macrolet:
            fun = new LExpressionMacro(outer,
                                       bdp->Cdr().Car(),
                                       bdp->Cdr().Cdr());
            break;
    }
    inner->AddFunBinding(symb, fun);

    do_bindings(dp->Cdr(), inner, outer, mode);
}

static void do_flet(const SReference &params,
                    IntelibContinuation& lf,
                    do_bindings_mode mode)
{
    LContextRef InnerContext(new LExpressionContext(lf.GetContext()));
    do_bindings(params.Car(), InnerContext, lf.GetContext(), mode);

    lf.PushTodo(IntelibContinuation::set_context, lf.GetContext());
    lf.PushTodo(IntelibContinuation::evaluate_progn, params.Cdr());
    lf.PushTodo(IntelibContinuation::set_context, InnerContext);
}

void LFunctionFlet::
Call(const SReference &params, IntelibContinuation& lf) const
{
    do_flet(params, lf, bindings_flet);
}

void LFunctionLabels::
Call(const SReference &params, IntelibContinuation& lf) const
{
    do_flet(params, lf, bindings_labels);
}

void LFunctionMacrolet::
Call(const SReference &params, IntelibContinuation& lf) const
{
    do_flet(params, lf, bindings_macrolet);
}

#endif

