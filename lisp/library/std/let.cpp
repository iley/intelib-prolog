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

DECLARE_SPECFORM(LFunctionLet, "#<FORM LET>", "LET")
DECLARE_SPECFORM(LFunctionLets, "#<FORM LET*>", "LET*")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

#include "../../../sexpress/slocatn.hpp"

static void eval_bind_values(const SReference& list, IntelibContinuation& lf,
                             const LContextRef &context,
                             const SReference &body, bool set_context)
{
    SExpressionCons *ls = list.DynamicCastGetPtr<SExpressionCons>();
    if(!ls) { // we don't check for empty list, just ignore dotted rest
        // list is over, the basis of the recursion is to schedule the
        // evaluation of the body and, possibly, the setting of the context

        lf.PushTodo(IntelibContinuation::evaluate_progn, body);
        if(set_context)
            lf.PushTodo(IntelibContinuation::set_context, context);

        return;
    }

    // on the onward way of recursion, we must schedule dynamic var
    //                         restoration, if any...

    SReference &lscar = ls->Car();

    LExpressionSymbol *symb;
    SReference *valref = 0;

    symb = lscar.DynamicCastGetPtr<LExpressionSymbol>();
    if(symb) {
        // this means it is not to be evaluated, will be
        // just bound to NIL... so, we leave the things as they are
    } else {
        // otherwise, there's the symbol and the expression...
        symb = lscar.Car().DynamicCastGetPtr<LExpressionSymbol>();
        if(!symb) throw IntelibX_lisp_not_a_symbol(ls->Car().Car());
        valref = &(lscar.Cdr().Car());
    }

    // check if the symbol is dynamic
    if(symb->IsDynamic()) {  // yes. So, schedule it's restoration
        lf.PushTodo(
            IntelibContinuation::assign_to,
            new SExpressionLocation(symb, &(symb->GetDynamicValueRef()))
        );
        lf.PushTodo(
            IntelibContinuation::quote_parameter,
            symb->GetDynamicValue()
        );
    }

    // now the rest of the list (the onward recursion actions complete)

    eval_bind_values(ls->Cdr(), lf, context, body, set_context);

    if(!valref) {
        if(symb->IsDynamic()) {
            symb->SetDynamicValue(*PTheEmptyList);
        } else {
            *(context->ProvideBinding(symb)) = *PTheEmptyList;
        }
    } else {
        SReference *location, superstruct;
        if(symb->IsDynamic()) {
           location = &(symb->GetDynamicValueRef());
           superstruct = lscar;
        } else {
           location = context->ProvideBinding(symb);
           superstruct = context;
        }
        lf.PushTodo(IntelibContinuation::assign_to,
            SReference(new SExpressionLocation(superstruct, location)));
        lf.PushTodo(IntelibContinuation::just_evaluate, *valref);
    }
}

static void do_let(const SReference &paramsv, IntelibContinuation& lf,
                   bool inits_within_context)
{
        // the very last step is the context restoration
    lf.PushTodo(IntelibContinuation::set_context, lf.GetContext());


#if 0
        // the next-to-last step is evaluation of the body
    lf.PushTodo(IntelibContinuation::evaluate_progn, paramsv.Cdr());
#endif

    LContextRef tempcontext(new LExpressionContext(lf.GetContext()));

#if 0
    if(!inits_within_context)
        lf.PushTodo(IntelibContinuation::set_context, tempcontext);
#endif

    eval_bind_values(paramsv.Car(), lf, tempcontext,
                     paramsv.Cdr(), !inits_within_context);

    if(inits_within_context)
        lf.PushTodo(IntelibContinuation::set_context, tempcontext);
}

void LFunctionLet::
Call(const SReference &paramsv, IntelibContinuation& lf) const
{
    do_let(paramsv, lf, false);
}

void LFunctionLets::
Call(const SReference &paramsv, IntelibContinuation& lf) const
{
    do_let(paramsv, lf, true);
}

#endif
