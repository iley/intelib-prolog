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
