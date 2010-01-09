//   InteLib                                    http://www.intelib.org
//   The file tools/numunif.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "../sexpress/sexpress.hpp"

const IntelibTypeId&
LispUnifyNumbericTypes(SReference &l1, SReference &l2)
{
    const IntelibTypeId *t1;
    const IntelibTypeId *t2;
    if((t1 = &(l1->TermType())) == (t2 = &(l2->TermType()))) return *t1;
    // We assume there are only SExpressionInt and SExpressionFloat around
    // This will likely change in a close future
    // For this time, however, this rule allows us to cast both args
    // to FLOAT and relax ;-) In fact, since the args have different
    // types, we only need to convert only the one which is integer
    if(*t1 == SExpressionInt::TypeId) {
        l1 = new SExpressionFloat(static_cast<intelib_float_t>(l1.GetInt()));
    } else
        if(*t2 == SExpressionInt::TypeId) {
            l2 = new SExpressionFloat(static_cast<intelib_float_t>(l2.GetInt()));
        }
    return SExpressionFloat::TypeId;
}
