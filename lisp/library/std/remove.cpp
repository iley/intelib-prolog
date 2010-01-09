//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/remove.cpp
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

DECLARE_CFUNCTION(LFunctionRemove, 2, 3, "#<FUNCTION REMOVE>", "REMOVE")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

#include "../../../sexpress/squeue.hpp"

class LExpressionRemoveIterator : public SExpressionGenericIterator {
    SReference predicate;
    SReference val;
    SReference src;
    SQueue res;
public:
    static IntelibTypeId TypeId;

    LExpressionRemoveIterator(const SReference &fun,
                              const SReference &aval,
                              const SReference &list)
        : SExpressionGenericIterator(TypeId),
          predicate(fun), val(aval), src(list), res()
    {}
    ~LExpressionRemoveIterator() {}

    void ScheduleTest(IntelibContinuation& lf) {}

    bool NeedAnotherIteration(IntelibContinuation& lf) const {
        return !src.IsEmptyList();
    }

    void ScheduleIteration(IntelibContinuation &lf) {
        lf.PushResult(predicate);
        lf.PushResult(val);
        lf.PushResult(src.Car());
        lf.PushTodo(2);
    }

    void CollectResultOfIteration(IntelibContinuation &lf) {
        SReference r;
        lf.PopResult(r);
        if(!LReference(r).IsTrue()) {
            res.Append(src.Car());
        }
        src = src.Cdr();
    }

    void ReturnFinalValue(IntelibContinuation &lf) {
        lf.RegularReturn(res);
    }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    class SString TextRepresentation() const
        { return "#<-REMOVE ITERATOR->"; }
#endif
};

IntelibTypeId
LExpressionRemoveIterator::TypeId(&SExpression::TypeId,true);


static LReference do_remove2(const LReference &item,
                             const SReference &list)
{
    SExpressionCons *dp = list.SimpleCastGetPtr<SExpressionCons>();
    if(!dp) return list;
    bool to_remove = item.IsEql(dp->Car());
    LReference rest = do_remove2(item, dp->Cdr());
    if(to_remove) return rest;
    if(rest.GetPtr() == dp->Cdr().GetPtr()) return list;
    return SReference(dp->Car(), rest);
}


// <item> <list> [<test>]
void LFunctionRemove::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(paramsc == 2) {
        lf.RegularReturn(do_remove2(paramsv[0], paramsv[1]));
    } else {
        SReference iter(
            new LExpressionRemoveIterator(paramsv[2], paramsv[0], paramsv[1])
        );
        lf.PushTodo(IntelibContinuation::generic_iteration, iter);
    }
}

#endif

