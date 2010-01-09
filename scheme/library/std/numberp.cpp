//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/numberp.cpp
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

DECLARE_CFUNCTION(SchFunctionNumberp, 1, 1, "#<FUNCTION NUMBER?>", "NUMBER?")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION

void SchFunctionNumberp::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    if(paramsv[0]->TermType() == SExpressionInt::TypeId
       || paramsv[0]->TermType() == SExpressionFloat::TypeId
    ) {
        lf.RegularReturn(*PTheSchemeBooleanTrue);
    } else {
        lf.RegularReturn(*PTheSchemeBooleanFalse);
    }
}

#endif

