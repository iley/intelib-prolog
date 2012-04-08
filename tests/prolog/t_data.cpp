//   InteLib                                    http://www.intelib.org
//   The file tests/prolog/t_data.cpp
//
//   Copyright (c) Ilya Strukov, 2011-2012
//
//
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
//
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.

#include <stdio.h>
#include "../tests.hpp"

#include "sexpress/iexcept.hpp"
#include "sexpress/sstring.hpp"
#include "prolog/prolog.hpp"
#include "sexpress/sexpress.hpp"

int main()
{
    try {
        TestSection("Data");
        {
            SListConstructor S;

            PlgAtom a("a");
            TESTTR("atom", a, "a");

            PlgVariable X("X");
            TESTTR("variable name", X, "X");

            TESTTR("term", a(X, X), "a(X, X)");
            TESTTR("conjunction", a(X, X) & a, "a(X, X),a");
            TESTTR("disjunction", a(X, X) | a, "a(X, X);a");
            TESTTR("compund expression", (a(X, X) & a(X) | a(X) & a(X, X)), "a(X, X),a(X);a(X),a(X, X)");

            TESTTR("clause", a(X) << a(X, X), "a(X):-a(X, X)");

            PlgAtom x("x"), x_alias("x");
            TESTB("atom equality", x.IsEql(x_alias));

            PlgVariable X_alias("X");
            TESTB("variable equality", X.IsEql(X_alias));
        }
        TestScore();
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
    return 0;
}


