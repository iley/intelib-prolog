//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/str/char.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "str_inc.h"

DECLARE_CFUNCTION(SFunctionChar,  2,  2,  "#<FUNCTION CHAR>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

#include <string.h>

void SFunctionChar::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    const char *s = paramsv[0].GetString();
    intelib_integer_t ind = paramsv[1].GetInt();
    char res = (ind >=0 && ind < (intelib_integer_t)strlen(s)) ? s[ind] : '\0';
    lf.RegularReturn(SReference(res));
}

#endif

