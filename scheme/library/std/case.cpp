//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/case.cpp
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

DECLARE_SPECFORM(SchFunctionCase, "#<FORM CASE>", "CASE")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION

static void DoPlace(const SReference &paramsv, IntelibContinuation& lf)
{
    if(paramsv.IsEmptyList()) {
        lf.PushTodo(IntelibContinuation::end_of_clauses);
        lf.PushTodo(IntelibContinuation::drop_result); // switch value
    } else {
        DoPlace(paramsv.Cdr(), lf);
        lf.PushTodo(IntelibContinuation::cond_clause, paramsv.Car().Cdr());
        SReference caar = paramsv.Car().Car();
        if(caar.DynamicCastGetPtr<SExpressionCons>()) {
            lf.PushTodo(SchemeContinuation::case_check, caar);
        } else {
            lf.PushTodo(IntelibContinuation::quote_parameter,
                        *PTheSchemeBooleanTrue);
            lf.PushTodo(IntelibContinuation::drop_result);
        }
    }
}

void SchFunctionCase::
Call(const SReference &paramsv, IntelibContinuation& lf) const
{
    DoPlace(paramsv.Cdr(), lf);
    lf.PushTodo(IntelibContinuation::just_evaluate, paramsv.Car());
}
#endif
