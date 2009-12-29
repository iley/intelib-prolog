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




#include "../sexpress/sstring.hpp"
#include "llambda.hpp"

#include "lsymbol.hpp"

#if INTELIB_DEBUG_PRINT >= 1
#endif

IntelibTypeId
LExpressionSymbol::TypeId(&SExpressionSpecialToken::TypeId, false);

/* NOTE NOTE NOTE Symbol is declared unchangeable
   although actually it is 
 */
/* rationale is that it should not be copied when casting from constant
   to non-constant S-expressions 
 */

static SReference& GetSymbolValue(LispContinuation *c, LExpressionSymbol *sym)
{
    SReference *lexval = 0;
    if(c->GetContext().GetPtr()) {
        lexval = c->GetContext()->GetBinding(sym);
    }
    if(lexval)
        return *lexval;
    else
        return sym->GetDynamicValueRef();
}


void LExpressionSymbol::Evaluate(class IntelibContinuation& lf)
{
    if(IsDynamic()) {
        if(IsConstant()) {
            lf.RegularReturn(GetDynamicValue());
        } else {
            lf.ReferenceReturn(GetDynamicValueRef(), this);
        }
    } else {
        SReference &res =
            GetSymbolValue(&static_cast<LispContinuation&>(lf), this); 
        if(!res.GetPtr())
            throw IntelibX_lisp_symbol_has_no_value(this);
        lf.ReferenceReturn(res, this);
    }
}

void LExpressionSymbol::GetApplicable(class SExpressionForm **form,
                                      SReference *function,
                                      IntelibContinuation &lf) const
{
    SReference val;
    LExpressionContext *ct =
       static_cast<LispContinuation&>(lf).GetContext().GetPtr();
    if(ct) {
        SReference *p = ct->GetFunBinding(this);
        if(p)
            val = *p;
    }
    if(!val.GetPtr())
        val = GetFunction();
    if(!val.GetPtr()) {
        *form = 0;
        *function = val;
    } else {
        SExpressionForm *fm = val.DynamicCastGetPtr<SExpressionForm>();
        if(!(*form = fm))
            *function = val;
    }
}

void LExpressionSymbol::SetDynamicValue(const SReference &val)
{
    if(is_constant) {
        throw IntelibX_cant_change_constant(SReference(this));
    }
    dynamic_value = val;
}

LSymbol::~LSymbol()
{
#if INTELIB_CLEAR_SYMBOLS == 1
    LReference none; // empty reference
    GetPtr()->SetDynamicValue(none);
    GetPtr()->SetFunction(none);
#endif
}

class LFunctionQuote : public SExpressionForm {
public:
    LFunctionQuote() : SExpressionForm() {}
    ~LFunctionQuote() {}
    virtual SString TextRepresentation() const
        { return "#<FORM QUOTE>"; }
    virtual void Call(const SReference &params, IntelibContinuation &lf) const
    {
        lf.RegularReturn(
            (static_cast<SExpressionCons*>(params.GetPtr()))->Car()
        );
    }
};

class LFunctionFunction : public SExpressionForm {
public:
    LFunctionFunction() : SExpressionForm() {}
    ~LFunctionFunction() {}
    virtual SString TextRepresentation() const
        { return "#<FORM FUNCTION>"; }
    virtual void Call(const SReference &params, IntelibContinuation &lf) const
    {
        INTELIB_ASSERT(params.Cdr().IsEmptyList(),
                       IntelibX_too_many_params(params));
        LReference ret;

        SExpressionSpecialToken *tk =
            params.Car().DynamicCastGetPtr<SExpressionSpecialToken>();
        if(tk) {
            SExpressionForm *f;
            SReference func;
            tk->GetApplicable(&f, &func, lf);
            if(!f) { 
                ret = func;
            }
        } else {
            // may be it's a lambda-list?
            SExpressionCons *dp = 
                params.Car().DynamicCastGetPtr<SExpressionCons>();
            if(dp) {
                if(dp->Car() == *PTheLispSymbolLambda) {
                    LReference tmp = dp->Cdr(); 
                    ret = LReference(new LExpressionLambda(lf.GetContext(),
                                                           tmp.Car(), 
                                                           tmp.Cdr()));
                }
            }
        }

        INTELIB_ASSERT(ret.GetPtr(),
                       IntelibX_no_associated_function(params.Car()));
        lf.RegularReturn(ret);
    }
};

LSymbolQuote::LSymbolQuote()
        : LSymbol("QUOTE")
{
    (*this)->SetFunction(LReference(new LFunctionQuote));
    PTheLispSymbolQuote = this;
}

LSymbolFunction::LSymbolFunction()
        : LSymbol("FUNCTION")
{
    (*this)->SetFunction(LReference(new LFunctionFunction));
    PTheLispSymbolFunction = this;
}

LSymbolLambda::LSymbolLambda()
        : LSymbol("LAMBDA")
{
    PTheLispSymbolLambda = this;
}

LSymbolNIL::LSymbolNIL()
        : LSymbol("NIL")
{
    GetPtr()->SetDynamicValue(*this);
    GetPtr()->SetConstant();
    PTheLispBooleanFalse = this;
    PTheEmptyList = this;
}

LSymbolT::LSymbolT()
        : LSymbol("T")
{
    GetPtr()->SetDynamicValue(*this);
    GetPtr()->SetConstant();
    PTheLispBooleanTrue = this;
}

LLibraryProvidedSymbols TheLibraryProvidedSymbols;

IntelibX_lisp_not_a_symbol::
IntelibX_lisp_not_a_symbol(SReference a_param)
        : IntelibX("Lisp: Not a symbol", a_param) {}

IntelibX_symbol_has_no_value::
IntelibX_symbol_has_no_value(SReference a_param)
        : IntelibX("Symbol has no value", a_param) {}

IntelibX_cant_change_constant::
IntelibX_cant_change_constant(SReference a_param)
        : IntelibX("Can not change a constant", a_param) {}

