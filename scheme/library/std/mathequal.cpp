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

DECLARE_CFUNCTION(SchFunctionMathequal, 2, 2, "#<FUNCTION = >", "=")
DECLARE_CFUNCTION(SchFunctionMathnotequal, 2, 2, "#<FUNCTION /= >", "/=")
DECLARE_CFUNCTION(SchFunctionMathgeq, 2, 2, "#<FUNCTION >= >", ">=")
DECLARE_CFUNCTION(SchFunctionMathleq, 2, 2, "#<FUNCTION <= >", "<=")
DECLARE_CFUNCTION(SchFunctionLessp, 2, 2, "#<FUNCTION LESSP>", "<")
DECLARE_CFUNCTION(SchFunctionGreaterp, 2, 2, "#<FUNCTION GREATERP>", ">")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION

#include <string.h>

static int lispdcmp(const SReference &left, const SReference &right)
{
    if(left->TermType().IsSubtypeOf(SExpressionString::TypeId)) {
        if(!right->TermType().IsSubtypeOf(SExpressionString::TypeId))
            throw IntelibX_not_a_string(right);
        return strcmp(left.GetString(), right.GetString());
    }
    SchReference op0 = left;
    SchReference op1 = right;
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


void SchFunctionMathequal::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    int res = (lispdcmp(paramsv[0], paramsv[1]) == 0);
    lf.RegularReturn(res ? *PTheSchemeBooleanTrue : *PTheSchemeBooleanFalse);
}

void SchFunctionMathnotequal::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    int res = (lispdcmp(paramsv[0], paramsv[1]) != 0);
    lf.RegularReturn(res ? *PTheSchemeBooleanTrue : *PTheSchemeBooleanFalse);
}

void SchFunctionMathgeq::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    int res = (lispdcmp(paramsv[0], paramsv[1]) >= 0);
    lf.RegularReturn(res ? *PTheSchemeBooleanTrue : *PTheSchemeBooleanFalse);
}

void SchFunctionMathleq::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    int res = (lispdcmp(paramsv[0], paramsv[1]) <= 0);
    lf.RegularReturn(res ? *PTheSchemeBooleanTrue : *PTheSchemeBooleanFalse);
}

void SchFunctionGreaterp::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    int res = (lispdcmp(paramsv[0], paramsv[1]) > 0);
    lf.RegularReturn(res ? *PTheSchemeBooleanTrue : *PTheSchemeBooleanFalse);
}

void SchFunctionLessp::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    int res = (lispdcmp(paramsv[0], paramsv[1]) < 0);
    lf.RegularReturn(res ? *PTheSchemeBooleanTrue : *PTheSchemeBooleanFalse);
}

#endif

