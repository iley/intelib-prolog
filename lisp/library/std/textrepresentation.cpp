//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/textrepresentation.cpp
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

DECLARE_CFUNCTION(LFunctionTextrepresentation, 1, 1,
                  "#<FUNCTION TEXTREPRESENTATION>",
                  "TEXTREPRESENTATION")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionTextrepresentation::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
#if INTELIB_TEXT_REPRESENTATIONS == 1
    lf.RegularReturn(LReference(paramsv[0]).TextRepresentation());
#else 
    lf.RegularReturn("#<INTELIB_TEXT_REPRESENTATIONS ARE OFF>");
#endif
}

#endif

