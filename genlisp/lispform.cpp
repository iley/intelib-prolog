//   InteLib                                    http://www.intelib.org
//   The file genlisp/lispform.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




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

