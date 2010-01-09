//   InteLib                                    http://www.intelib.org
//   The file tests/prolog/t_data.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include <stdio.h>
#include "tests.hpp"

#include "sexpress/iexcept.hpp"
#include "sexpress/sstring.hpp"
#include "prolog/prolog.hpp"



void poc()
{
#if INTELIB_DEBUG_COUNTERS == 1
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}

int main()
{
    try {
        poc();
        TestSection("PrologData");
        {
            PlgSymbol john("john");
            PlgSymbol george("george");
            PlgSymbol ann("ann");
            PlgSymbol brian("brian");
            PlgSymbol parent("parent");

            parent(john, george) <<= true;
            parent(john, ann) <<= true;
            parent(ann, brian) <<= true;

            parent(1,2,3,4) <<= true;

            TESTTR("facts", parent->GetProcedure(2),
                            "((parent(john, george)) "
                             "(parent(john, ann)) "
                             "(parent(ann, brian)))");

            TESTTR("different_arity", parent->GetProcedure(4),
                                     "((parent(1, 2, 3, 4)))");

            TESTB("undefined_arity0", !parent->GetProcedure(0).GetPtr());
            TESTB("undefined_arity1", !parent->GetProcedure(1).GetPtr());
            TESTB("undefined_arity3", !parent->GetProcedure(3).GetPtr());
            TESTB("undefined_arity500", !parent->GetProcedure(500).GetPtr());


            PlgSymbol relative("relative");
            PlgVariable X("X");
            PlgVariable Y("Y");
            PlgVariable Z("Z");

            relative(X, Y) <<= parent(X, Y);
            relative(X, Y) <<= parent(Y, X);
            relative(X, Y) <<= relative(X, Z), relative(Z, Y);

            TESTTR("longer_clauses", relative->GetProcedure(2),
                   "((relative(X, Y) parent(X, Y)) "
                    "(relative(X, Y) parent(Y, X)) "
                    "(relative(X, Y) relative(X, Z) relative(Z, Y)))");
        }
        TestScore();
        poc();
    }
    catch(IntelibX &x) {
        printf("\nCaught IntelibX: %s\n", x.Description() );
        if(x.Parameter().GetPtr()) {
            printf("%s\n", x.Parameter()->TextRepresentation().c_str());
        }
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    poc();
    return 0;
}


