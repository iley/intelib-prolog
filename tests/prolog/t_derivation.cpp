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
SReference &Nil = *(GetEmptyList());

int main()
{
    try {
        TestSection("Derivation");
        {
            using namespace PlgStdLib;
            SListConstructor S;
            PlgAtom d("d"), x("x"), sin("sin"), cos("cos"), pow("pow");
            PlgVariable X("X"), Y("Y"), DY("DY"),
                        A("A"), B("B"),
                        DA("DA"), DB("DB"), D("D"),
                        P1("P1"), P2("P2"),
                        E("E"), E1("E1");

            d(X, X, 1) <<= cut;
            d(A + B, X, DA + DB) <<= d(A, X, DA) & d(B, X, DB) & cut;
            d(A - B, X, DA - DB) <<= d(A, X, DA) & d(B, X, DB) & cut;
            d(A * B, X, DA*B+DB*A) <<= d(A, X, DA) & d(B, X, DB) & cut;
            d(A / B, X, (DA*B-DB*A)/pow(B,2)) <<= d(A, X, DA) & d(B, X, DB) & cut;
            d(-A, X, -DA) <<= d(A, X, DA) & cut;
            d(pow(X,E), X, E*pow(X,E1)) <<= (E1 ^= E - SReference(1)) & cut;
            d(sin(Y), X, cos(Y)*DY) <<= d(Y, X, DY) & cut;
            d(cos(Y), X, -sin(Y)*DY) <<= d(Y, X, DY) & cut;
            d(Y, X, 0) <<= truth;

            SReference one(1), zero(0);
            Ok(d(x, x, D), D, (S|one));
            Ok(d(1, x, D), D, (S|zero));
            Ok(d(x*x, x, D), D, (S|one * x + one * x));
            Ok(d(sin(x), x, D), D, (S|cos(x) * one));
            Ok(d(sin(x) * cos(x), x, D), D, (S| cos(x) * one * cos(x) + (-sin(x) * one *sin(x))));
            Ok(d(one / sin(X), x, D), D, (S|zero * sin(x) - cos(x) * one * one / pow(sin(x), 2)));

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
