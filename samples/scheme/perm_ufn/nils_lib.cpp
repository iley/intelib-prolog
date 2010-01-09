//   InteLib                                    http://www.intelib.org
//   The file samples/scheme/perm_ufn/nils_lib.cpp
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
#include <intelib/scheme/schsymb.hpp>


#include "perm.hpp"



extern "C" {
    void nils_lib_init_function(SchReference *ref);
}


void nils_lib_init_function(SchReference *ref)
{
    SchSymbol CREATE_PG("CREATE_PG");
    SchSymbol NEXT_PERMUTATION("NEXT_PERMUTATION");

    CREATE_PG->SetGlobalValue(create_permutation_generator);
    NEXT_PERMUTATION->SetGlobalValue(next_permutation);

    SchListConstructor L;
    *ref = (L|CREATE_PG, NEXT_PERMUTATION);
}
