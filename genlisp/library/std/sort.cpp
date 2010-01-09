//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/std/sort.cpp
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

DECLARE_CFUNCTION(SFunctionSort, 2, 3, "#<FUNCTION SORT>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

static SReference do_reverse(SReference ls)
{
   SReference res(*PTheEmptyList);
   while(!ls.IsEmptyList()) {
       res = SReference(ls.Car(), res);
       ls = ls.Cdr();
   } 
   return res;
}

static SReference append2(const SReference &l1, const SReference &l2)
{
    if(l1.IsEmptyList()) return l2;
    return SReference(l1.Car(), append2(l1.Cdr(), l2));
}

class SFunctionSort_DoMerge : public SExpressionFunction {
    SReference predicate;
    SReference selector;
public:
    SFunctionSort_DoMerge(const SReference &pred, const SReference &sel)
        : SExpressionFunction(3, 4), predicate(pred), selector(sel) {}
    ~SFunctionSort_DoMerge() {}

    void ScheduleComparision(IntelibContinuation &lf,
                             const SReference &el1,
                             const SReference &el2) const
    {
        lf.PushTodo(2);
        lf.PushResult(predicate);
        if(selector.GetPtr()) {
            lf.PushTodo(IntelibContinuation::evaluate_prepared,
                SReference(selector, SReference(el2, *PTheEmptyList))
            );
            lf.PushTodo(IntelibContinuation::evaluate_prepared,
                SReference(selector, SReference(el1, *PTheEmptyList))
            );
        } else {
            lf.PushTodo(IntelibContinuation::quote_parameter, el2);
            lf.PushTodo(IntelibContinuation::quote_parameter, el1);
        }
    }

    // <result_list> <list1> <list2> [<car_comparision_result>] 
    virtual void DoApply(int paramsc, 
                         const SReference paramsv[],
                         IntelibContinuation &lf) const
    {
#if 0
        printf("*** %d [%s] [%s] [%s]\n", paramsc,
               paramsv[0]->TextRepresentation().c_str(),
               paramsv[1]->TextRepresentation().c_str(),
               paramsv[2]->TextRepresentation().c_str());
#endif

        SReference result_list = paramsv[0];
        SReference ls1 = paramsv[1];
        SReference ls2 = paramsv[2];

        if(ls1.IsEmptyList()) {
            lf.RegularReturn(append2(do_reverse(result_list), ls2));
            return;
        }
        if(ls2.IsEmptyList()) {
            lf.RegularReturn(append2(do_reverse(result_list), ls1));
            return;
        }

        if(paramsc == 3) {
            // no comparision result... just reschedule ourselves
            // after it will be available
        } else { // 4
            SReference comp_result(paramsv[3]);
            // okay, let's first change the lists
            if(comp_result != lf.False()) {
                // pick from the first
                result_list = SReference(ls1.Car(), result_list);
                ls1 = ls1.Cdr();
            } else {
                // pick from the second
                result_list = SReference(ls2.Car(), result_list);
                ls2 = ls2.Cdr();
            }
        }
        // here we do rescheduling ourselves, yeah...
        lf.PushResult(SReference(this));
        lf.PushResult(result_list);
        lf.PushResult(ls1);
        lf.PushResult(ls2);
        if(!ls1.IsEmptyList() && !ls2.IsEmptyList()) {
            lf.PushTodo(4);
            ScheduleComparision(lf, ls1.Car(), ls2.Car());
        } else {
            lf.PushTodo(3);
        }
    }
#if INTELIB_TEXT_REPRESENTATIONS == 1    
    virtual SString TextRepresentation() const { return "#<-SORT-DO-MERGE->"; }
#endif
};

static void split_list(const SReference &src,
                       SReference &dst1, SReference &dst2)
{
    if(src.IsEmptyList()) return;
    dst1 = SReference(src.Car(), dst1);
    if(src.Cdr().IsEmptyList()) return;
    dst2 = SReference(src.Cdr().Car(), dst2);
    split_list(src.Cdr().Cdr(), dst1, dst2);
}

// <list_to_sort> <predicate> [<selector>]
void SFunctionSort::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
//    printf("*** SORT: %s\n", paramsv[0]->TextRepresentation().c_str());

    if(paramsv[0].IsEmptyList() || paramsv[0].Cdr().IsEmptyList()) {
        lf.RegularReturn(paramsv[0]); // empty or 1 element, already sorted
        return;
    }
    SReference ls1 = *PTheEmptyList, ls2 = *PTheEmptyList;
    split_list(paramsv[0], ls1, ls2);

    SReference predicate(paramsv[1]);
    SReference selector;
    if(paramsc > 2) selector = paramsv[2];
    SReference funmerge(new SFunctionSort_DoMerge(predicate, selector));


    // the last thing to do will be Merge (of 3 arguments)
    lf.PushResult(funmerge);
    lf.PushResult(*PTheEmptyList); // result list
    lf.PushTodo(3);  // the remaining two args will be produced by... hmm..

    SReference funsort(this);

    // scheduling sorting of the second one
    lf.PushTodo(IntelibContinuation::evaluate_prepared,
        SReference(funsort,
            SReference(ls2,
                SReference(predicate,
                    paramsc>2 ? SReference(selector, *PTheEmptyList)
                              : *PTheEmptyList
                )
            )
        )
    );

    // scheduling sorting of the first one
    lf.PushTodo(IntelibContinuation::evaluate_prepared,
        SReference(funsort,
            SReference(ls1,
                SReference(predicate,
                    paramsc>2 ? SReference(selector, *PTheEmptyList)
                              : *PTheEmptyList
                )
            )
        )
    );

}

#endif

