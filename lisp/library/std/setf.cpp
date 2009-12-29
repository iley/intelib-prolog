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

DECLARE_SPECFORM(LFunctionSetf, "#<FORM SETF>", "SETF")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionSetf::
Call(const SReference &params, IntelibContinuation& lf) const
{
    LReference lastresult = *PTheEmptyList;
    SReference curform = params;
    while(!curform.IsEmptyList()) {
        SReference loc = curform.Car();
        SReference to_eval = curform.Cdr().Car();
        curform = curform.Cdr().Cdr();

        lf.PushTodo(IntelibContinuation::assign_location);
        lf.PushTodo(IntelibContinuation::just_evaluate, loc);
        if(curform.IsEmptyList()) // the last one is to be returned
            lf.PushTodo(LispContinuation::duplicate_last_result);
        lf.PushTodo(IntelibContinuation::just_evaluate, to_eval);
    }
}
#endif
