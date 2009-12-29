// +-------------------------------------------------------------------------+
// |               I__n__t__e__L__i__b           0.6.21 development          |
// | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
// |                                                                         |
// | This is free software. The library part is available under              |
// |                               GNU LESSER GENERAL PUBLIC LICENSE v.2.1.  |
// | GNU LGPL v2.1 is found in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
// |     Please see also docs/readme.txt and visit http://www.intelib.org    |
// |                                                                         |
// | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
// +-------------------------------------------------------------------------+




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

