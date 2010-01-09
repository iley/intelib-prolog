//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/m/atan.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "m_inc.h"

DECLARE_CFUNCTION(SFunctionAtan, 1, 2, "#<FUNCTION ATAN>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION


void SFunctionAtan::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    intelib_float_t res =
        paramsc == 1 ? atan(paramsv[0].GetFloat()) :
                       atan2(paramsv[0].GetFloat(), paramsv[1].GetFloat());
    lf.RegularReturn(res); 
}

#endif

