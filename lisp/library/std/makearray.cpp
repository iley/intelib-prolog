//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/makearray.cpp
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

DECLARE_CFUNCTION(LFunctionMakearray, 0, 1,
                  "#<FUNCTION MAKEARRAY>", "MAKEARRAY")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

#include "../../../sexpress/svector.hpp"

void LFunctionMakearray::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    int dim = paramsc > 0 ? paramsv[0].GetInt() : 0;
    SVector v(dim);
    for(int i = 0; i < dim; i++) v[i] = *PTheEmptyList;
    lf.RegularReturn(v);
}

#endif

