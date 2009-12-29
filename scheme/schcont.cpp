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




#include "../sexpress/iexcept.hpp"
#include "../sexpress/sstring.hpp"
#include "scheme.hpp"
#include "schcont.hpp"
#include "schsymb.hpp"
#include "schfun.hpp"


////////////////////////////////////////////////////////

IntelibTypeId SchExpressionContext::TypeId(&SExpression::TypeId, false);

SchExpressionContext::SchExpressionContext()
    : SExpression(TypeId)
{}

SchExpressionContext::SchExpressionContext(SchContextRef a_prev)
    : SExpression(TypeId), prev(a_prev)
{}

SchExpressionContext::~SchExpressionContext()
{}

void SchExpressionContext::AddBinding(const SchExpressionSymbol *symb,
                                      const SReference& val)
{
    *tbl.AddBinding(symb) = val;
}

SReference*
SchExpressionContext::GetBinding(const SchExpressionSymbol *symb) const
{
    SReference *res = tbl.GetBinding(symb);
    if(res) return res;
    if(prev.GetPtr()) {
        return prev->GetBinding(symb);
    } else {
        return 0;
    }
}

SReference*
SchExpressionContext::ProvideBinding(const SchExpressionSymbol *symb)
{
    SReference *res = tbl.GetBinding(symb);
    if(!res) res = tbl.AddBinding(symb);
    return res;
}

SReference SchExpressionContext::GetAllSymbols() const
{
    SReference ret;
    if(prev.GetPtr()) {
        ret = prev->GetAllSymbols();
    } else {
        ret = *PTheEmptyList;
    }
    IntelibBindingsSet::Iterator iter(tbl);
    const SExpressionLabel *key;
    SReference t;
    while(iter.GetNext(key, t)) {
        ret = SReference(key, ret);
    }
    return ret;
}

SString SchExpressionContext::TextRepresentation() const
{
    return "#<SCHEME-CONTEXT>";
}

/////////////////////////////////////////////////

void SchemeContinuation::CustomCommand(int opcode, const SReference& param)
{
    switch(opcode) {
        case case_check:
            CaseCheck(param);
            break;
        default:
                // this will just throw an exception
            IntelibContinuation::CustomCommand(opcode, param);
    }
}

static bool eqv_memberp(const SReference &a, const SReference &ls)
{
    SExpressionCons *dp = ls.DynamicCastGetPtr<SExpressionCons>();
    if(!dp) return false;
    return a.IsEql(dp->Car()) || eqv_memberp(a, dp->Cdr());
}

void SchemeContinuation::CaseCheck(const SReference& expr)
{
    SReference val;
    PopResult(val);
    if(eqv_memberp(val, expr)) {
        // that's it, push true
        PushResult(*PTheSchemeBooleanTrue);
    } else {
        // that's not it, return value back
        PushResult(val);
        PushResult(*PTheSchemeBooleanFalse);
    }
}

#if 0
void SchemeContinuation::JustEvaluate(const SReference& expr)
{
    if(!expr.GetPtr()) throw IntelibX_unexpected_unbound_value();
    const IntelibTypeId *t;
    for(t=&(expr->TermType()); t; t=t->Prev()) {
        if(*t == SExpressionCons::TypeId) {
            EvaluateForm(static_cast<SExpressionCons*>(expr.GetPtr()));
            return;
        } else
        if(*t == SchExpressionSymbol::TypeId) {
            SchExpressionSymbol *sym =
                static_cast<SchExpressionSymbol*>(expr.GetPtr());
            SReference &res = GetSymbolValue(sym); 
            if(!res.GetPtr())
                throw IntelibX_scheme_symbol_has_no_value(expr);
            ReferenceReturn(res, expr);
            return;
        }
    }
    // no parent means that it's a regular constant...
    // it is always self-evaluated
    PushResult(expr);
}
#endif

#if 0
void SchemeContinuation::DoFunctionCall(const SReference &fun_ref,
                                     int paramscount,
                                     const SReference *paramsvector)
{
    SExpressionFunction *fun =
        fun_ref.DynamicCastGetPtr<SExpressionFunction>();
    if(!fun) {
        throw IntelibX_scheme_not_a_function(fun_ref);
    }
    fun->Apply(paramscount, paramsvector, *this);
}
#endif

/*
   in order not to forget:
   SExpressionSpecialToken::Evaluate() MUST NOT be called from within
   the EvaluateForm() method, because this produces malfunctioning in
   case there's the assign_location command on top of the TODO stack, 
   that is, the 1st argument of SETF! is being evaluated.

 */

void SchemeContinuation::EvaluateForm(const SExpressionCons *form)
{
#if 0 // INTELIB_CONTINUATION_KEEPS_STACK_INFO == 1
    int keepstack_position = todo_stack_pointer;
#endif
    SExpressionSpecialToken *sym =
        form->Car().DynamicCastGetPtr<SExpressionSpecialToken>();
    if(sym) {
        SExpressionForm *f;
        SReference func;
        sym->GetApplicable(&f, &func, *this);
        if(f) { // special symbol
            f->Call(form->Cdr(), *this);
        } else {
            if(!func.GetPtr()) {
                throw IntelibX_scheme_symbol_has_no_value(sym);
            }
            PushResult(func);
            SExpressionCons *cdr =
                form->Cdr().DynamicCastGetPtr<SExpressionCons>();
            if(cdr) {
                PlaceFormToStack(cdr, 1);
            } else {
                PushTodo(0);
            }
        }
    } else {
        PlaceFormToStack(form, 0);
    }
#if 0 // INTELIB_CONTINUATION_KEEPS_STACK_INFO == 1
    todo_stack[keepstack_position].stack_info = SReference(*form);
#endif
}

SReference& SchemeContinuation::
GetSymbolValue(SchExpressionSymbol *sym) const
{
    SReference *lexval = 0;
    if(GetContext().GetPtr()) {
        lexval = GetContext()->GetBinding(sym);
    }
    if(lexval)
        return *lexval;
    else
        return sym->GetGlobalValue();
}

SReference SchemeContinuation::
GetSymbolValue(const SchExpressionSymbol *sym) const
{
    SReference *lexval = 0;
    if(GetContext().GetPtr()) {
        lexval = GetContext()->GetBinding(sym);
    }
    if(lexval)
        return *lexval;
    else
        return sym->GetGlobalValue();
}

#if 0
IntelibX_scheme_not_a_function::
IntelibX_scheme_not_a_function(SReference a_param) 
    : IntelibX("scheme: not a function", a_param) {}
#endif

IntelibX_scheme_not_a_context::
IntelibX_scheme_not_a_context(SReference a_param) 
    : IntelibX("scheme: not a context", a_param) {}

IntelibX_scheme_symbol_has_no_value::
IntelibX_scheme_symbol_has_no_value(SReference a_param) 
    : IntelibX("scheme: symbol has no value", a_param) {}

