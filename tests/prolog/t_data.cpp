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


