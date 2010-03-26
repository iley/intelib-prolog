//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/std/mapcar.cpp
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

DECLARE_CFUNCTION(SFunctionMapcar, 2, -1, "#<FUNCTION MAPCAR>")
DECLARE_CFUNCTION(SFunctionFor_each, 2, -1, "#<FUNCTION FOR-EACH>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

class SExpressionMapIterator : public SExpressionGenericIterator {
    SReference fun;
    int params;
    SReference *rests;
    SReference results;
    bool done;
    bool not_first;
public:

    SExpressionMapIterator(const SReference &a_fun,
                           int a_params,
                           const SReference *a_rests,
                           bool accumulate_results = true)
        : fun(a_fun), params(a_params),
          results(accumulate_results ? *PTheEmptyList : SReference()),
          done(false), not_first(false)
    {
        rests = new SReference[params];
        for(int i=0; i<params; i++) {
            rests[i] = a_rests[i];
            if(rests[i].IsEmptyList()) done = true;
        }
    }

private:
    ~SExpressionMapIterator() { delete[] rests; }

    void DoIteration(IntelibContinuation &lf) {
        if(not_first) {
            SReference res;
            lf.PopResult(res);
            if(results.GetPtr())
                results = SReference(res, results);
        }
        not_first = true;
        if(!done) {
            lf.PushTodo(IntelibContinuation::generic_iteration, this);
            lf.PushResult(fun);
    
#if 0  // in CLISP, unemptied lists in mapcar are no error
            bool all_emptied = true;
#endif
            bool any_emptied = false;
            for(int i = 0; i < params; i++) {
                lf.PushResult(rests[i].Car());
                rests[i] = rests[i].Cdr();
                if(rests[i].IsEmptyList())
                    any_emptied = true;
#if 0
                else
                    all_emptied = false;
#endif
            }
            lf.PushTodo(params);
            if(any_emptied) {
#if 0
                if(all_emptied) {
                    done = true;
                } else {
                    SReference r(*PTheEmptyList);
                    for(int i = params-1; i >= 0; i--)
                        r = SReference(rests[i], r);
                    throw IntelibX("map: lists of different length", r);
                }
#endif
                done = true;
            }
        } else {  // done
            if(results.GetPtr()) {
                SReference res = *PTheEmptyList;
                reverse(results, res);
                lf.PushResult(res);
            } else {
                lf.ReturnUnspecified();
            }
        }
    }

    static void reverse(const SReference &res, SReference &t) {
        if(res.IsEmptyList()) return;
        t = SReference(res.Car(), t);
        reverse(res.Cdr(), t);
    }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    class SString TextRepresentation() const { return "#<-MAP ITERATOR->"; }
#endif
};


void SFunctionMapcar::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SReference iter(new SExpressionMapIterator(paramsv[0],
                                               paramsc-1,
                                               paramsv+1));
    lf.PushTodo(IntelibContinuation::generic_iteration, iter);
}

void SFunctionFor_each::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SReference iter(new SExpressionMapIterator(paramsv[0],
                                               paramsc-1,
                                               paramsv+1,
                                               false));
    lf.PushTodo(IntelibContinuation::generic_iteration, iter);
}
#endif
