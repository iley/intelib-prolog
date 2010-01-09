//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/while.cpp
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

DECLARE_SPECFORM(LFunctionWhile, "#<FUNCTION WHILE>", "WHILE")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

#include "../../../sexpress/slocatn.hpp"

class LExpressionWhileIterator : public SExpressionGenericIterator {
    SReference test;
    SReference body;
public:
    static IntelibTypeId TypeId;

    LExpressionWhileIterator(const SReference &atest, const SReference &abody)
        : SExpressionGenericIterator(TypeId), test(atest), body(abody)
    {}
    ~LExpressionWhileIterator() {}

    void ScheduleTest(IntelibContinuation& lf)
    {
        lf.PushTodo(IntelibContinuation::just_evaluate, test);
    }

    bool NeedAnotherIteration(IntelibContinuation& lf) const {
        SReference res;
        lf.PopResult(res);
        return res.GetPtr() != PTheLispBooleanFalse->GetPtr();
    }

    void ScheduleIteration(IntelibContinuation &lf) {
        ScheduleTest(lf);
        lf.PushTodo(IntelibContinuation::drop_result);
        lf.PushTodo(IntelibContinuation::evaluate_progn, body);
    }

    void CollectResultOfIteration(IntelibContinuation &lf) {
        // nothing to do
    }

    void ReturnFinalValue(IntelibContinuation &lf) {
        lf.RegularReturn(*PTheEmptyList);
    }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    class SString TextRepresentation() const { return "#<-WHILE ITERATOR->"; }
#endif
};

IntelibTypeId LExpressionWhileIterator::TypeId(&SExpression::TypeId,true);

void LFunctionWhile::
Call(const SReference &paramsv, IntelibContinuation& lf) const
{
        // the very last step is the context restoration
    lf.PushTodo(IntelibContinuation::set_context, lf.GetContext());

    SReference &test = paramsv.Car();
    SReference &body = paramsv.Cdr();


    LExpressionWhileIterator *iter = new LExpressionWhileIterator(test, body);
    lf.PushTodo(IntelibContinuation::generic_iteration, SReference(iter));
    iter->ScheduleTest(lf);
}

#endif
