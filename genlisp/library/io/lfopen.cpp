//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/io/lfopen.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "io_inc.h"

DECLARE_CFUNCTION(SFunctionLfopen,  1,  2,  "#<FUNCTION LFOPEN>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionLfopen::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    const char *fname = paramsv[0].GetString();
    const char *mode = (paramsc == 2) ? paramsv[1].GetString() : "r";
    FILE *fl = fopen(fname, mode);
    if (!fl) {
        lf.RegularReturn(lf.False());
    } else {
        lf.RegularReturn(
            paramsc == 1 ?
            SReference(new SExpressionStreamTextInput(fl)) :
            SReference(new SExpressionStreamFile(fl))
        );
    }
}

#endif

