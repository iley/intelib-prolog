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
            //Hooks::EnableAll();
            //PrintContextEnabled = true;

            SListConstructor S;
            PlgAtom member("member"),
                solution("solution"),
                noattack("noattack"),
                pos("pos"),
                pattern("pattern"),
                queens("queens");

            PlgVariableName H("H"), T("T"),
                X("X"), Y("Y"),
                X1("X1"), Y1("Y1"),
                Y2("Y2"), Y3("Y3"), Y4("Y4"), Y5("Y5"), Y6("Y6"), Y7("Y7"), Y8("Y8");

            PlgDatabase db;

            PlgAtom ne = PlgStdLib::int_not_equal;
            db.Add( member(X, H^T) <<= (X ^= H) | member(X, T) );
            db.Add( solution(Nil) );
            db.Add( solution(pos(X,Y) ^ T) <<= solution(T) & member(Y, (S|1,2,3,4)) & noattack(pos(X,Y), T) );
            db.Add( noattack(X, Nil) );
            db.Add( noattack(pos(X,Y), pos(X1,Y1) ^ T) <<=
                ne(Y, Y1) &
                ne(Y1 - Y, X1 - X) &
                ne(Y1 - Y, X - X1) &
                noattack(pos(X,Y), T) );
            //db.Add( pattern((S|pos(1, Y1), pos(2, Y2), pos(3, Y3), pos(4, Y4), pos(5, Y5), pos(6, Y6), pos(7, Y7), pos(8, Y8))) );
            db.Add( pattern((S|pos(1, Y1), pos(2, Y2), pos(3, Y3), pos(4, Y4))) );
            db.Add( queens(X) <<= pattern(X) & solution(X) );

            Ok(db, queens(X), X,
                (S| (S|pos(1,3), pos(2,1), pos(3,4), pos(4,2)),
                    (S|pos(1,2), pos(2,4), pos(3,1), pos(4,3))));

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
