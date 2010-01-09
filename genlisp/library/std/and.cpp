//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/std/and.cpp
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

DECLARE_SPECFORM(SFunctionAnd, "#<FORM AND>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

static void DoPlace(const SReference &paramsv, IntelibContinuation& lf)
{
    SReference &cdr = paramsv.Cdr();
    if(cdr.IsEmptyList()) {
        lf.PushTodo(IntelibContinuation::end_of_clauses);
        lf.PushTodo(IntelibContinuation::just_evaluate, paramsv.Car());
    } else {
        DoPlace(cdr, lf);
        lf.PushTodo(IntelibContinuation::bail_on_false);
        lf.PushTodo(IntelibContinuation::just_evaluate, paramsv.Car());
    }
}

void SFunctionAnd::
Call(const SReference &paramsv, IntelibContinuation& lf) const
{
#if 0
    /* this hack was for the Lisp version only; Scheme should report an
       error for (and) form. If it appears to matter once, we should
       perhaps introduce a separate version of the function for Lisp,
       e.g., SFunctionAnd_relaxed
     */
    if(paramsv.IsEmptyList()) {
        lf.RegularReturn(lf.True());
        return;
    }
#endif
    DoPlace(paramsv, lf);
}
#endif
