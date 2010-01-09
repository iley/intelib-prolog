//   InteLib                                    http://www.intelib.org
//   The file genlisp/extvar.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "extvar.hpp"

#if INTELIB_TEXT_REPRESENTATIONS == 1
#include <stdio.h> // for snprintf

SString SExpressionIndirect::TextRepresentation() const
{
    char buf[32];
    snprintf(buf, sizeof(buf), "#<EXTVAR @%lx>", (long)Addr());
    return SString(buf);
}
#endif




#if 0
void SExpressionIndirectAccessor::
Call(const SReference& param, IntelibContinuation &lf) const
{
    SExpressionIndirect *p =
        static_cast<SExpressionIndirect*>(param.GetPtr());
    lf.AgentReturn(p->Value(), p);
}
#endif



IntelibTypeId
SExpressionExternVariable::TypeId(&SExpressionSpecialToken::TypeId, false);

SExpressionExternVariable::SExpressionExternVariable(SExpressionIndirect *p)
#if INTELIB_TEXT_REPRESENTATIONS == 1
    : SExpressionSpecialToken(TypeId, p->TextRepresentation().c_str()),
#else
    : SExpressionSpecialToken(TypeId, "#<-ext->"),
#endif
    indirect(p)
{}

void SExpressionExternVariable::Evaluate(class IntelibContinuation& lf)
{
    SExpressionIndirect *p =
        static_cast<SExpressionIndirect*>(indirect.GetPtr());
    lf.AgentReturn(p->Value(), p);
}

