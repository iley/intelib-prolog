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

