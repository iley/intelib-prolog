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

DECLARE_CFUNCTION(LFunctionLreduce, 2, 3, "#<FUNCTION LREDUCE>", "LREDUCE")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION


class LExpressionLreduceIterator : public SExpressionGenericIterator {
    SReference fun;
    SReference val;
    SReference rest;
public:
    static IntelibTypeId TypeId;

    LExpressionLreduceIterator(const SReference &afun,
                               const SReference &aval,
                               const SReference &arest)
        : SExpressionGenericIterator(TypeId),
          fun(afun), val(aval), rest(arest)
    {}
    ~LExpressionLreduceIterator() {}

    void ScheduleTest(IntelibContinuation& lf) {}

    bool NeedAnotherIteration(IntelibContinuation& lf) const {
        return !rest.IsEmptyList();
    }

    void ScheduleIteration(IntelibContinuation &lf) {
        lf.PushResult(fun);
        lf.PushResult(val);
        lf.PushResult(rest.Car());
        lf.PushTodo(2);
        rest = rest.Cdr();
    }

    void CollectResultOfIteration(IntelibContinuation &lf) {
        lf.PopResult(val);
    }

    void ReturnFinalValue(IntelibContinuation &lf) {
        lf.RegularReturn(val);
    }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    class SString TextRepresentation() const
        { return "#<-LREDUCE ITERATOR->"; }
#endif
};

IntelibTypeId
LExpressionLreduceIterator::TypeId(&SExpression::TypeId,true);

void LFunctionLreduce::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SReference ini(paramsc == 3 ? paramsv[2] : paramsv[1].Car());
    SReference lst(paramsc == 3 ? paramsv[1] : paramsv[1].Cdr());
    SReference fun(paramsv[0]);
    SReference iter(new LExpressionLreduceIterator(fun, ini, lst));
    lf.PushTodo(IntelibContinuation::generic_iteration, iter);
}

#endif

