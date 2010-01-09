//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/setcar.cpp
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

DECLARE_CFUNCTION(SchFunctionSetcar, 2, 2, "#<FUNCTION SET-CAR!>", "SET-CAR!")
DECLARE_CFUNCTION(SchFunctionSetcdr, 2, 2, "#<FUNCTION SET-CDR!>", "SET-CDR!")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION

#include "../../../sexpress/slocatn.hpp"

enum setmode { set_car, set_cdr };

static void do_set(const SReference paramsv[], IntelibContinuation& lf, int m)
{
    SExpressionCons *dp = paramsv[0].DynamicCastGetPtr<SExpressionCons>();
    INTELIB_ASSERT(dp, IntelibX_not_a_cons(paramsv[0]));
    lf.PushTodo(IntelibContinuation::return_unspecified);
    SReference loc(
       new SExpressionLocation(paramsv[0],
                               &(m == set_car ? dp->Car() : dp->Cdr()))
    );
    lf.PushTodo(IntelibContinuation::assign_to, loc);
    //lf.PushTodo(IntelibContinuation::quote_parameter, paramsv[1]);
    lf.PushResult(paramsv[1]);
}

void SchFunctionSetcar::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    do_set(paramsv, lf, set_car);
}
void SchFunctionSetcdr::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    do_set(paramsv, lf, set_cdr);
}
#endif

