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




#include "std_inc.h"

DECLARE_CFUNCTION(SFunctionApply, 2, -1, "#<FUNCTION APPLY>")

#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION
void SFunctionApply::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SReference argslist = paramsv[paramsc - 1];
    for(int i = paramsc-2; i>=1; i--) {
        argslist = SReference(paramsv[i], argslist);
    }
    lf.PushResult(paramsv[0]);
    int argscount = 0;
    while(!argslist.IsEmptyList()) {
        lf.PushResult(argslist.Car());
        argslist = argslist.Cdr();
        argscount++;
    }
    lf.PushTodo(argscount);
}
#endif
