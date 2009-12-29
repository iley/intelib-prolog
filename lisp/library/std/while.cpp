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
