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

DECLARE_CFUNCTION(LFunctionMaparray, 2, 2, "#<FUNCTION MAPARRAY>", "MAPARRAY")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

#include "../../../sexpress/svector.hpp"

// RETURN THE SIZE !!!

class LExpressionMaparrayIterator : public SExpressionGenericIterator {
    SReference vecref;
    SExpressionVector *vec;
    SReference fun;
    int next_index;
public:
    static IntelibTypeId TypeId;

    LExpressionMaparrayIterator(const SReference &afun,
                                const SReference &avecref,
                                SExpressionVector *avec)
        : SExpressionGenericIterator(TypeId),
          vecref(avecref), vec(avec), fun(afun), next_index(0)
    {}
    ~LExpressionMaparrayIterator() {}

    void ScheduleTest(IntelibContinuation& lf) {}

    bool NeedAnotherIteration(IntelibContinuation& lf) const {
        return next_index < vec->Size();
    }

    void ScheduleIteration(IntelibContinuation &lf) {
        lf.PushResult(fun);
        lf.PushResult((*vec)[next_index]);
        next_index++;
        lf.PushTodo(1);
    }

    void CollectResultOfIteration(IntelibContinuation &lf) {}

    void ReturnFinalValue(IntelibContinuation &lf) {
        lf.RegularReturn(vec->Size());
    }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    class SString TextRepresentation() const
        { return "#<-MAPARRAY ITERATOR->"; }
#endif
};

IntelibTypeId
LExpressionMaparrayIterator::TypeId(&SExpression::TypeId,true);

void LFunctionMaparray::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{    // (maparray (function fn) hash)
    SExpressionVector *vect =
        paramsv[1].DynamicCastGetPtr<SExpressionVector>();
    INTELIB_ASSERT(vect, IntelibX_not_a_vector(paramsv[1]));

    lf.PushTodo(IntelibContinuation::generic_iteration,
                SReference(new LExpressionMaparrayIterator(paramsv[0],
                                                           paramsv[1],
                                                           vect)));
}

#endif

