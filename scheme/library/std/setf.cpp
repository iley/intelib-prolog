//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/setf.cpp
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

DECLARE_SPECFORM(SchFunctionSetf, "#<FORM SETF!>", "SETF!")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION

void SchFunctionSetf::
Call(const SReference &paramsv, IntelibContinuation& lf) const
{
    // we must evaluate the value, and then assign 
    // finally, we should leave something on the stack
    lf.PushTodo(IntelibContinuation::return_unspecified);
    lf.PushTodo(IntelibContinuation::assign_location);
    lf.PushTodo(IntelibContinuation::just_evaluate, paramsv.Car());
    lf.PushTodo(IntelibContinuation::just_evaluate, paramsv.Cdr().Car());
}
#endif
