//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/maparray.cpp
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
    LExpressionMaparrayIterator(const SReference &afun,
                                const SReference &avecref,
                                SExpressionVector *avec)
        : vecref(avecref), vec(avec), fun(afun), next_index(0)
    {}

private:
    ~LExpressionMaparrayIterator() {}

    void DoIteration(IntelibContinuation &lf) {
        if(next_index < vec->Size()) {
            lf.PushTodo(IntelibContinuation::generic_iteration, this);
            lf.PushTodo(IntelibContinuation::drop_result);
            lf.PushResult(fun);
            lf.PushResult((*vec)[next_index]);
            next_index++;
            lf.PushTodo(1);
        } else {
            lf.RegularReturn(vec->Size());
        }
    }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    class SString TextRepresentation() const
        { return "#<-MAPARRAY ITERATOR->"; }
#endif
};

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

