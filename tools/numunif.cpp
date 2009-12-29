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
