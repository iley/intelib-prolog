//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/while.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2010
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


class LExpressionWhileIterator : public SExpressionGenericIterator {
    SReference test;
    SReference body;
    bool first;
public:

    LExpressionWhileIterator(const SReference &atest, const SReference &abody)
        : test(atest), body(abody), first(true)
    {}

private:
    ~LExpressionWhileIterator() {}

    void DoIteration(IntelibContinuation &lf) {
        if(first) {
            // only schedule a test and a return 
            lf.PushTodo(IntelibContinuation::generic_iteration, this);
            lf.PushTodo(IntelibContinuation::just_evaluate, test);
            first = false;
            return;
        }

        SReference res;
        lf.PopResult(res);
        bool done = (res.GetPtr() == PTheLispBooleanFalse->GetPtr());

        if(!done) {
            lf.PushTodo(IntelibContinuation::generic_iteration, this);
            lf.PushTodo(IntelibContinuation::just_evaluate, test);
            lf.PushTodo(IntelibContinuation::drop_result);
            lf.PushTodo(IntelibContinuation::evaluate_progn, body);
        } else {
            lf.RegularReturn(*PTheEmptyList);
        }
    }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    class SString TextRepresentation() const { return "#<-WHILE ITERATOR->"; }
#endif
};

void LFunctionWhile::
Call(const SReference &paramsv, IntelibContinuation& lf) const
{
#if 0  // I don't know why the hell this is needed
       // If it's here for a reason, please remove the #if and 
       // add a comment about this

        // the very last step is the context restoration
    lf.PushTodo(IntelibContinuation::set_context, lf.GetContext());
#endif

    SReference &test = paramsv.Car();
    SReference &body = paramsv.Cdr();

    LExpressionWhileIterator *iter = new LExpressionWhileIterator(test, body);
    lf.PushTodo(IntelibContinuation::generic_iteration, SReference(iter));
}

#endif
