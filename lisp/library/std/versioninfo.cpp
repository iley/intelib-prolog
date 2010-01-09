//   InteLib                                    http://www.intelib.org
//   The file lisp/library/std/versioninfo.cpp
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

DECLARE_CFUNCTION(LFunctionVersioninfo, 0, 0, "#<FUNCTION VERSIONINFO>",
                  "VERSIONINFO")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION
void LFunctionVersioninfo::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    LListConstructor L;
    lf.RegularReturn((L|the_intelib_title,
                      the_intelib_copyright,
                      the_intelib_version_id));
}

#endif

