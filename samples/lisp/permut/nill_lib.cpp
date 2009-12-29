// +-------------------------------------------------------------------------+
// |               I__n__t__e__L__i__b           0.6.21 development          |
// | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
// |                                                                         |
// | This is free software.  See GNU GENERAL PUBLIC LICENSE v.2 for details. |
// | GNU GPL v2  is  found  in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
// |     Please see also docs/readme.txt and visit http://www.intelib.org    |
// |                                                                         |
// | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
// +-------------------------------------------------------------------------+




#include <intelib/sexpress/sexpress.hpp>
#include <intelib/sexpress/sstring.hpp>
#include <intelib/lisp/lsymbol.hpp>
#include <intelib/lisp/lform.hpp>


#include "perm.hpp"



extern "C" {
    void nill_lib_init_function(LReference *ref);
}


void nill_lib_init_function(LReference *ref)
{
    LFunctionalSymbol<LFunctionCreatepg> CREATE_PG("CREATE_PG");
    LFunctionalSymbol<LFunctionNextpermutation> 
                              NEXT_PERMUTATION("NEXT_PERMUTATION");
    LListConstructor L;
    *ref = (L|CREATE_PG, NEXT_PERMUTATION);
}
