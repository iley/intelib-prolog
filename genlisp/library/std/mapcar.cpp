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

DECLARE_CFUNCTION(SFunctionMapcar, 2, -1, "#<FUNCTION MAPCAR>")
DECLARE_CFUNCTION(SFunctionFor_each, 2, -1, "#<FUNCTION FOR-EACH>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

class SExpressionMapIterator : public SExpressionGenericIterator {
    SReference fun;
    int params;
    SReference *rests;
    SReference results;
    bool done;
public:
    static IntelibTypeId TypeId;

    SExpressionMapIterator(const SReference &a_fun,
                           int a_params,
                           const SReference *a_rests,
                           bool accumulate_results = true)
        : SExpressionGenericIterator(TypeId),
          fun(a_fun), params(a_params),
          results(accumulate_results ? *PTheEmptyList : SReference()),
          done(false)
    {
        rests = new SReference[params];
        for(int i=0; i<params; i++) {
            rests[i] = a_rests[i];
            if(rests[i].IsEmptyList()) done = true;
        }
    }
    ~SExpressionMapIterator() { delete[] rests; }

    bool NeedAnotherIteration(IntelibContinuation& lf) const {
        return !done;
    }

    void ScheduleIteration(IntelibContinuation &lf) {
        lf.PushResult(fun);

        bool all_emptied = true;
        bool any_emptied = false;
        for(int i = 0; i < params; i++) {
            lf.PushResult(rests[i].Car());
            rests[i] = rests[i].Cdr();
            if(rests[i].IsEmptyList())
                any_emptied = true;
            else
                all_emptied = false;
        }
        lf.PushTodo(params);
        if(any_emptied) {
            if(all_emptied) {
                done = true;
            } else {
                SReference r(*PTheEmptyList);
                for(int i = params-1; i >= 0; i--)
                    r = SReference(rests[i], r);
                throw IntelibX("scheme: map: lists of different length", r);
            }
        }
    }

    void CollectResultOfIteration(IntelibContinuation &lf) {
        SReference res;
        lf.PopResult(res);
        if(results.GetPtr())
            results = SReference(res, results);
    }

    static void reverse(const SReference &res, SReference &t) {
        if(res.IsEmptyList()) return;
        t = SReference(res.Car(), t);
        reverse(res.Cdr(), t);
    }

    void ReturnFinalValue(IntelibContinuation &lf) {
        if(results.GetPtr()) {
            SReference res = *PTheEmptyList;
            reverse(results, res);
            lf.PushResult(res);
        } else {
            lf.ReturnUnspecified();
        }
    }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    class SString TextRepresentation() const { return "#<-MAP ITERATOR->"; }
#endif
};

IntelibTypeId SExpressionMapIterator::TypeId(&SExpression::TypeId,true);



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
