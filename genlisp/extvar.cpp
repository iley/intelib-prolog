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

