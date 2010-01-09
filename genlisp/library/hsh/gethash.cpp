//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/hsh/gethash.cpp
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

DECLARE_CFUNCTION(SFunctionGethash,  2,  3,  "#<FUNCTION GETHASH>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

void SFunctionGethash::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{           // (... key table [default])
    SExpressionHashTable *table =
        paramsv[1].DynamicCastGetPtr<SExpressionHashTable>();
    INTELIB_ASSERT(table, IntelibX_not_a_hash_table(paramsv[1]));
    SReference *itemp = &(table->GetItemPosition(paramsv[0]));
    if(itemp->GetPtr() == SExpressionHashTable::EmptySlotMark.GetPtr()) {
        if(paramsc == 3) {
            // *itemp = paramsv[2]; // this was an error!
            lf.RegularReturn(paramsv[2]);
            return;
        }
    }
    lf.ReferenceReturn(*itemp, paramsv[1]);
}

#endif

