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

