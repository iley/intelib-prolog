//   InteLib                                    http://www.intelib.org
//   The file samples/lisp/perm_ufn/nill_lib.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




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
    LSymbol CREATE_PG("CREATE_PG");
    LSymbol NEXT_PERMUTATION("NEXT_PERMUTATION");

    CREATE_PG->SetFunction(create_permutation_generator);
    NEXT_PERMUTATION->SetFunction(next_permutation);

    LListConstructor L;
    *ref = (L|CREATE_PG, NEXT_PERMUTATION);
}
