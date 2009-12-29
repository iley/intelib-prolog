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
#include "scheme.hpp"
#include "schcont.hpp"
#include "schfun.hpp"
#include "schsymb.hpp"


IntelibTypeId
SchExpressionSymbol::TypeId(&SExpressionSpecialToken::TypeId, false); 


void SchExpressionSymbol::Evaluate(class IntelibContinuation& lf)
{
    SReference &res =
        static_cast<SchemeContinuation&>(lf).GetSymbolValue(this); 
    if(!res.GetPtr())
        throw IntelibX_scheme_symbol_has_no_value(this);
    lf.ReferenceReturn(res, this);
}

void SchExpressionSymbol::GetApplicable(class SExpressionForm **form,
                                        SReference *function,
                                        IntelibContinuation &lf) const
{
    SReference res =
        static_cast<SchemeContinuation&>(lf).GetSymbolValue(this);
    if(!res.GetPtr()) {
        *form = 0;
        *function = SReference();
        return;
    }
    if(!(*form = res.DynamicCastGetPtr<SExpressionForm>()))
        *function = res;
}
 
class SchFormQuote : public SExpressionForm {
public:
    SchFormQuote() : SExpressionForm() {}
    ~SchFormQuote() {}
    virtual SString TextRepresentation() const
        { return "#<FORM QUOTE>"; }
    virtual void Call(const SReference &params, IntelibContinuation &lf) const
    {
        lf.PushTodo(SchemeContinuation::quote_parameter, params.Car());
    }
};

SchSymbolQuote::SchSymbolQuote()
    : SchSymbol("QUOTE", SReference(new SchFormQuote))
{
    PTheSchemeSymbolQuote = this;
}

SchLabelTrue::SchLabelTrue()
    : SLabel("#T")
{
    PTheSchemeBooleanTrue = this;
}

SchLabelFalse::SchLabelFalse()
    : SLabel("#F")
{
    PTheSchemeBooleanFalse = this;
}

SchLabelEmptyList::SchLabelEmptyList()
    : SLabel("()")
{
    PTheEmptyList = this;
}

SchLibraryProvidedSymbols TheSchLibraryProvidedSymbols;

SchSymbol &QUOTE = TheSchLibraryProvidedSymbols.Quote;

IntelibX_scheme_not_a_symbol::
IntelibX_scheme_not_a_symbol(SReference a_param)
        : IntelibX("Not a scheme symbol", a_param) {}
