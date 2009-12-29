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

DECLARE_CFUNCTION(LFunctionRreduce, 2, 3, "#<FUNCTION RREDUCE>", "RREDUCE")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION


// <fun> <list> [<initial>]
void LFunctionRreduce::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    SReference fun = paramsv[0];
    SReference tmp = paramsv[1];
    SReference ini;
    if(paramsc > 2) ini = paramsv[2];
    while(!tmp.IsEmptyList()) {
        SReference &tmpcdr = tmp.Cdr();
        if(paramsc == 2 && tmpcdr.IsEmptyList()) {
            lf.PushResult(tmp.Car());
            break;
        }
        lf.PushResult(fun);
        lf.PushResult(tmp.Car());
        lf.PushTodo(2);
        tmp = tmpcdr;
    }
    if(paramsc > 2)
        lf.PushResult(ini);
}

#endif

