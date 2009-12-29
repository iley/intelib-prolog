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

DECLARE_CFUNCTION(LFunctionFuncall, 1, -1, "#<FUNCTION FUNCALL>", "FUNCALL")

#ifdef INTELIB_LISP_LIBRARY_IMPLEMENTATION

static void place_to_results(int count, int paramsc,
                             const SReference *paramsv,
                             IntelibContinuation& lf)
{
    if(count >= paramsc) return;
    SReference save = paramsv[paramsc - 1 - count];
    place_to_results(count+1, paramsc, paramsv, lf);
    lf.PushResult(save);
}


void LFunctionFuncall::
DoApply(int paramsc, const SReference paramsv[], IntelibContinuation& lf) const
{
    // we can't just place paramsv[] elements to the results stack
    // because the paramsv vector IS the part of the stack!
    place_to_results(0, paramsc, paramsv, lf);
    lf.PushTodo(paramsc - 1);
}

#endif

