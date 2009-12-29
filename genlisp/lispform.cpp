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
#include "conteval.hpp"
#include "lispform.hpp"

IntelibTypeId SExpressionFunction::TypeId(&SExpression::TypeId);
IntelibTypeId SExpressionForm::TypeId(&SExpression::TypeId);

///////////////////////////////////////////////////////////////////

void SExpressionFunction::Apply(int paramc, const SReference *paramv,
                                IntelibContinuation &cont) const
{
    if(min_param != -1 && paramc < min_param)
        throw IntelibX_too_few_params(this);
    if(max_param != -1 && paramc > max_param)
        throw IntelibX_too_many_params(this);
    DoApply(paramc, paramv, cont);
}

///////////////////////////////////////////////////////////////////

IntelibTypeId
SExpressionUserCFunction::TypeId(&SExpressionFunction::TypeId);

void SExpressionUserCFunction::DoApply(int paramsc, 
                                       const SReference *paramsv,
                                       IntelibContinuation &lf) const
{
    lf.RegularReturn(fun(paramsc, paramsv));
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString SExpressionUserCFunction::TextRepresentation() const
{
    return "#<USER-DEFINED FUNCTION>";
}
#endif

///////////////////////////////////////////////////////////////////


IntelibX_too_few_params::
IntelibX_too_few_params(SReference a_param) 
    : IntelibX("to few params", a_param) {}

IntelibX_too_many_params::
IntelibX_too_many_params(SReference a_param) 
    : IntelibX("to many params", a_param) {}

IntelibX_invalid_arguments::
IntelibX_invalid_arguments(SReference a_param) 
    : IntelibX("invalid arguments", a_param) {}

