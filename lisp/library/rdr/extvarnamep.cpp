//   InteLib                                    http://www.intelib.org
//   The file lisp/library/rdr/extvarnamep.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "rdr_inc.h"

DECLARE_CFUNCTION(LFunctionExtvarnamep, 1, 1,
                  "#<FUNCTION EXTVARNAMEP>",
                  "EXTVARNAMEP")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

void LFunctionExtvarnamep::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SExpressionExtvarname *ca =
        paramsv[0].DynamicCastGetPtr<SExpressionExtvarname>();
    lf.RegularReturn(ca ? *PTheLispBooleanTrue : *PTheLispBooleanFalse);
}

#endif

