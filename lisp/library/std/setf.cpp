//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/setf.cpp
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
