//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/hsh/maphash.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2010
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "hsh_inc.h"

DECLARE_CFUNCTION(SFunctionMaphash,  2,  2,  "#<FUNCTION MAPHASH>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

class SExpressionMaphashIterator : public SExpressionGenericIterator {
    SReference fun;
    SExpressionHashTable::Iterator iter;
    SReference nextval;
public:

    SExpressionMaphashIterator(const SReference &a_fun,
                               SExpressionHashTable *hash)
        : fun(a_fun), iter(*hash)
    {
        nextval = iter.GetNext();
    }
    ~SExpressionMaphashIterator() {}

    void DoIteration(IntelibContinuation &lf) {
        if(nextval.GetPtr()) {
            lf.PushTodo(IntelibContinuation::generic_iteration, this);
            lf.PushTodo(IntelibContinuation::drop_result);
            lf.PushResult(fun);
            lf.PushResult(nextval.Car());
            lf.PushResult(nextval.Cdr());
            lf.PushTodo(2);
            nextval = iter.GetNext();
        }
    }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    class SString TextRepresentation() const
        { return "#<-MAPHASH ITERATOR->"; }
#endif
};

void SFunctionMaphash::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SExpressionHashTable *h =
        paramsv[1].DynamicCastGetPtr<SExpressionHashTable>();
    INTELIB_ASSERT(h, IntelibX_not_a_hash_table(paramsv[1]));
    SReference iter(new SExpressionMaphashIterator(paramsv[0], h));
    lf.PushTodo(IntelibContinuation::generic_iteration, iter);
}
#endif
