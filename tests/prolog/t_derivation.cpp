//   InteLib                                    http://www.intelib.org
//   The file tests/prolog/t_queens.cpp
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
#include <time.h>
#include "../../sexpress/iexcept.hpp"
#include "../../sexpress/sexpress.hpp"
#include "../../sexpress/sstring.hpp"
#include "../../prolog/prolog.hpp"
#include "../../prolog/utils.hpp"
#include "../tests.hpp"
#include "plgtest.hpp"

SListConstructor S;
SReference &Nil = *PTheEmptyList;

int main()
{
    try {
        TestSection("Queens");
        {
            using namespace PlgStdLib;
            SListConstructor S;
            PlgAtom d("d"), sum("sum"), prod("prod"), x("x");
            PlgVariable X("X"), Y("Y"),
                        A("A"), B("B"),
                        DA("DA"), DB("DB"), D("D"),
                        P1("P1"), P2("P2"),
                        R("R"), P("P");

            PlgDatabase db;

            db.Add( d(X, X, 1) <<= cut );
            db.Add( d(X, Y, 0) <<= atomic(X) & cut );
            db.Add( d(A + B, X, D) <<=
                    d(A, X, DA) &
                    d(B, X, DB) &
                    sum(DA, DB, D)
            );

            db.Add( d(A * B, X, D) <<=
                    d(A, X, DA) &
                    d(B, X, DB) &
                    prod(A, DB, P1) &
                    prod(DA, B, P2) &
                    sum(P1, P2, D) &
                    cut
            );

            db.Add( sum(A, 0, A) <<= cut );
            db.Add( sum(0, B, B) <<= cut );
            db.Add( sum(A, B, R) <<= integer(A) & integer(B) & R.is(A + B) & cut );
            db.Add( sum(A, B, A + B) );

            db.Add( prod(X, 0, 0) <<= cut );
            db.Add( prod(0, X, 0) <<= cut );
            db.Add( prod(A, 1, A) <<= cut );
            db.Add( prod(1, B, B) <<= cut );
            db.Add( prod(A, B, P) <<= integer(A) & integer(B) & P.is(A * B) & cut );
            db.Add( prod(A, B, A * B) );

            Ok(db, d(x*x, x, D), D, (S|x+x));
            SReference two(2), one(1);
            Ok(db, d(two * (x + one), x, D), D, (S|2));
            Ok(db, d(x + one, x, D), D, (S|1));
            Ok(db, d(x + x + one, x, D), D, (S|2));
            Ok(db, d(x + two * (x + one), x, D), D, (S|3));

            TestScore();
        }
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
