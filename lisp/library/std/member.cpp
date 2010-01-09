//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/member.cpp
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

DECLARE_CFUNCTION(LFunctionMember, 2, 3, "#<FUNCTION MEMBER>", "MEMBER")
DECLARE_CFUNCTION(LFunctionMemberp, 2, 3, "#<FUNCTION MEMBERP>", "MEMBERP")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

static SReference
do_member2(const SReference &elem, const SReference &ls, bool nonpredicate)
{
    SExpressionCons *dp = ls.DynamicCastGetPtr<SExpressionCons>();
    while(dp) {
        bool check = LReference(dp->Car()).IsEql(elem);
        if(check) {
            return nonpredicate ? SReference(dp) : *PTheLispBooleanTrue;
        }
        dp = dp->Cdr().DynamicCastGetPtr<SExpressionCons>();
    }
    return *PTheLispBooleanFalse;
}

static void do_member3(const SReference &elem, const SReference &ls,
                       const SReference &predicate,
                       const SReference &memberfunc,
                       IntelibContinuation &lf,
                       bool nonpredicate)
{
    if(ls.IsEmptyList()) {
        lf.RegularReturn(*PTheLispBooleanFalse);
        return;
    }
    lf.PushTodo(IntelibContinuation::end_of_clauses);
    lf.PushTodo(IntelibContinuation::evaluate_prepared,
                SReference(memberfunc,
                    SReference(elem,
                        SReference(ls.Cdr(),
                            SReference(predicate, *PTheEmptyList)
                        )
                    )
                ));
    lf.PushTodo(IntelibContinuation::cond_clause,
                SReference(
                    nonpredicate ?
                        SReference(~LReference(ls)) : *PTheLispBooleanTrue,
                    *PTheEmptyList
                ));
    lf.PushTodo(IntelibContinuation::evaluate_prepared,
                SReference(predicate,
                    SReference(elem,
                        SReference(ls.Car(), *PTheEmptyList)
                    )
                ));
}


void LFunctionMember::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(paramsc == 2) {
        lf.RegularReturn(do_member2(paramsv[0], paramsv[1], true));
    } else {
        do_member3(paramsv[0], paramsv[1], paramsv[2], 
                   SReference(this), lf, true);
    }
}

void LFunctionMemberp::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(paramsc == 2) {
        lf.RegularReturn(do_member2(paramsv[0], paramsv[1], false));
    } else {
        do_member3(paramsv[0], paramsv[1], paramsv[2], 
                   SReference(this), lf, false);
    }
}
#endif

