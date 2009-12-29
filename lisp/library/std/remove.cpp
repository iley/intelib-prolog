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

