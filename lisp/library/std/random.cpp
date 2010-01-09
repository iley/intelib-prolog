//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/random.cpp
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

DECLARE_CFUNCTION(LFunctionRandom, 1, 1, "#<FUNCTION RANDOM>", "RANDOM")
#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
#include <stdlib.h>
void LFunctionRandom::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    LReference res((int)
           (((intelib_float_t)(paramsv[0].GetInt())*rand()/(RAND_MAX+1.0))));
    lf.RegularReturn(res);
}
#endif

