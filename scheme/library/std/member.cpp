//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/member.cpp
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

DECLARE_CFUNCTION(SchFunctionMember, 2, 2, "#<FUNCTION MEMBER>", "MEMBER")
DECLARE_CFUNCTION(SchFunctionMemberp, 2, 2, "#<FUNCTION MEMBER?>", "MEMBER?")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION

static SchReference
MemberImplementation(const SReference paramsv[])
{
    SchListConstructor L;
    SExpressionCons *dp =
        paramsv[1].DynamicCastGetPtr<SExpressionCons>();
    while(dp) {
        bool check = SchReference(dp->Car()).IsEqual(paramsv[0]);
        if(check) {
            return SchReference(dp);
        }
        dp = dp->Cdr().DynamicCastGetPtr<SExpressionCons>();
    }
    return *PTheSchemeBooleanFalse;
}



void SchFunctionMember::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    lf.RegularReturn(MemberImplementation(paramsv));
}

void SchFunctionMemberp::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SchReference res = MemberImplementation(paramsv);
    if(res.IsTrue()) res = *PTheSchemeBooleanTrue;
    lf.RegularReturn(res);
}
#endif

