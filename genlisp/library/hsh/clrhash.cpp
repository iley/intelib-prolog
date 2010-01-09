//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/hsh/clrhash.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "hsh_inc.h"

DECLARE_CFUNCTION(SFunctionClrhash,  1,  1,  "#<FUNCTION CLRHASH>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionClrhash::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SExpressionHashTable *table =
        paramsv[0].DynamicCastGetPtr<SExpressionHashTable>();
    INTELIB_ASSERT(table, IntelibX_not_a_hash_table(paramsv[0]));
    table->Clear();
    lf.RegularReturn(paramsv[0]);
}

#endif

