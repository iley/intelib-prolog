//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/std/if.cpp
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

DECLARE_SPECFORM(SFunctionIf, "#<FORM IF>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION
void SFunctionIf::
Call(const SReference &paramsv, IntelibContinuation& lf) const
{
    SReference &r2 = paramsv.Cdr();
    SReference &r3 = r2.Cdr();
    lf.PushTodo(IntelibContinuation::end_of_clauses);
    if(r3.IsEmptyList()) {
        lf.PushTodo(IntelibContinuation::quote_parameter,
                    lf.False());
    } else {
        lf.PushTodo(IntelibContinuation::just_evaluate, r3.Car());
    }
    lf.PushTodo(IntelibContinuation::cond_clause,
        SReference(r2.Car(), *PTheEmptyList));
    lf.PushTodo(IntelibContinuation::just_evaluate, paramsv.Car());
}
#endif
