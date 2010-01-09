//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/hsh/remhash.cpp
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

DECLARE_CFUNCTION(SFunctionRemhash,  2,  2,  "#<FUNCTION REMHASH>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionRemhash::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SExpressionHashTable *table =
        paramsv[1].DynamicCastGetPtr<SExpressionHashTable>();
    INTELIB_ASSERT(table, IntelibX_not_a_hash_table(paramsv[1]));
    lf.ReturnBoolean(table->RemoveItem(paramsv[0]));
}

#endif

