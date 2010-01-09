//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/std/progn.cpp
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

DECLARE_SPECFORM(SFunctionProgn, "#<FORM PROGN>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION
void SFunctionProgn::
Call(const SReference &paramsv, IntelibContinuation& lf) const
{
    lf.PushTodo(IntelibContinuation::evaluate_progn, paramsv);
}
#endif
