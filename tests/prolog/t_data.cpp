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
#include "sexpress/sexpress.hpp"


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
            SListConstructor S;

            PlgAtom a = "a";
            TESTTR("atom", a, "a");

            PlgVariableName X = "X";
            TESTTR("variable name", X, "X");

            TESTTR("term", a(X, X), "a(X, X)");
            TESTTR("conjunction", a(X, X) & a, "a(X, X), a");
            TESTTR("disjunction", a(X, X) | a, "a(X, X); a");
            TESTTR("compund expression 1", (a(X, X) & a(X) | a(X) & a(X, X)), "a(X, X), a(X); a(X), a(X, X)");
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


