//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/mathequal.cpp
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

DECLARE_CFUNCTION(LFunctionMathequal, 2, 2, "#<FUNCTION mathequal>", "=")
DECLARE_CFUNCTION(LFunctionMathnotequal, 2, 2, "#<FUNCTION mathnotequal>", "/=")
DECLARE_CFUNCTION(LFunctionMathgeq, 2, 2, "#<FUNCTION mathgeq>", ">=")
DECLARE_CFUNCTION(LFunctionMathleq, 2, 2, "#<FUNCTION mathleq>", "<=")
DECLARE_CFUNCTION(LFunctionLessp, 2, 2, "#<FUNCTION LESSP>", "<")
DECLARE_CFUNCTION(LFunctionGreaterp, 2, 2, "#<FUNCTION GREATERP>", ">")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

#include <string.h>

static int lispdcmp(const SReference &left, const SReference &right)
{
    if(left->TermType().IsSubtypeOf(SExpressionString::TypeId)) {
        if(!right->TermType().IsSubtypeOf(SExpressionString::TypeId))
            throw IntelibX_not_a_string(right);
        return strcmp(left.GetString(), right.GetString());
    }
    LReference op0 = left;
    LReference op1 = right;
    const IntelibTypeId& t = LispUnifyNumbericTypes(op0, op1);
    if(t == SExpressionInt::TypeId) {
        return op0.GetInt() - op1.GetInt();
    } else {
        if(t == SExpressionFloat::TypeId) {
            float f = op0.GetFloat() - op1.GetFloat();
            if(f < 0) return -1;
            if(f > 0) return 1;
            return 0;
        } else {
            op0.GetFloat();
            op1.GetFloat(); 
                // one of them must have thrown the exception
            throw IntelibX_bug();
        }
    }
}


void LFunctionMathequal::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    int res = (lispdcmp(paramsv[0], paramsv[1]) == 0);
    lf.RegularReturn(res ? *PTheLispBooleanTrue : *PTheLispBooleanFalse);
}

void LFunctionMathnotequal::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    int res = (lispdcmp(paramsv[0], paramsv[1]) != 0);
    lf.RegularReturn(res ? *PTheLispBooleanTrue : *PTheLispBooleanFalse);
}

void LFunctionMathgeq::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    int res = (lispdcmp(paramsv[0], paramsv[1]) >= 0);
    lf.RegularReturn(res ? *PTheLispBooleanTrue : *PTheLispBooleanFalse);
}

void LFunctionMathleq::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    int res = (lispdcmp(paramsv[0], paramsv[1]) <= 0);
    lf.RegularReturn(res ? *PTheLispBooleanTrue : *PTheLispBooleanFalse);
}

void LFunctionGreaterp::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    int res = (lispdcmp(paramsv[0], paramsv[1]) > 0);
    lf.RegularReturn(res ? *PTheLispBooleanTrue : *PTheLispBooleanFalse);
}

void LFunctionLessp::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    int res = (lispdcmp(paramsv[0], paramsv[1]) < 0);
    lf.RegularReturn(res ? *PTheLispBooleanTrue : *PTheLispBooleanFalse);
}

#endif

