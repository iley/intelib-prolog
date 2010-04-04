//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/lreduce.cpp
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

DECLARE_CFUNCTION(LFunctionLreduce, 2, 3, "#<FUNCTION LREDUCE>", "LREDUCE")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION


class LExpressionLreduceIterator : public SExpressionGenericIterator {
    SReference fun;
    SReference val;
    SReference rest;
    bool not_first;
public:

    LExpressionLreduceIterator(const SReference &afun,
                               const SReference &aval,
                               const SReference &arest)
        : fun(afun), val(aval), rest(arest), not_first(false)
    {}

private:
    ~LExpressionLreduceIterator() {}

    void DoIteration(IntelibContinuation &lf) {
        if(not_first)
            lf.PopResult(val);
        not_first = true;
        if(!rest.IsEmptyList()) {
            lf.PushTodo(IntelibContinuation::generic_iteration, this);
            lf.PushResult(fun);
            lf.PushResult(val);
            lf.PushResult(rest.Car());
            lf.PushTodo(2);
            rest = rest.Cdr();
        } else {
            lf.RegularReturn(val);
        }
    }


#if INTELIB_TEXT_REPRESENTATIONS == 1
    class SString TextRepresentation() const
        { return "#<-LREDUCE ITERATOR->"; }
#endif
};

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

