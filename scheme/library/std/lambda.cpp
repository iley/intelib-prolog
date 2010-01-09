//   InteLib                                    http://www.intelib.org
//   The file scheme/library/std/lambda.cpp
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

DECLARE_SPECFORM(SchFunctionLambda, "#<FORM LAMBDA>", "LAMBDA")

#ifdef INTELIB_SCHEME_LIBRARY_IMPLEMENTATION
#include "../../schfun.hpp"
void SchFunctionLambda::
Call(const SReference &paramsv, IntelibContinuation& lf) const
{
     SReference res(new SchExpressionLambda(lf.GetContext(),
                                            paramsv.Car(),
                                            paramsv.Cdr()));
     lf.RegularReturn(res);
}
#endif
