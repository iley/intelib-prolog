//   InteLib                                    http://www.intelib.org
//   The file tests/prolog/t_sort.cpp
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
        using namespace PlgStdLib;

        SListConstructor S;
        PlgDatabase db;
        PlgVariable X("X"), Y("Y"), Z("Z");

        TestSection("Standard Library");

        Ok(db, member(1, (S|1)));
        Ok(db, member(1, (S|1,2,3)));
        Ok(db, member(2, (S|1,2,3)));
        Ok(db, member(3, (S|1,2,3)));
        Fail(db, member(4, (S|1,2,3)));
        Fail(db, member(1, Nil));

        Fail(db, nope(member(1, (S|1))));
        Fail(db, nope(member(1, (S|1,2,3))));
        Fail(db, nope(member(2, (S|1,2,3))));
        Fail(db, nope(member(3, (S|1,2,3))));
        Ok(db, nope(member(4, (S|1,2,3))));
        Ok(db, nope(member(1, Nil)));

        Ok(db, append((S|1,2,3), (S|4,5), (S|1,2,3,4,5)));
        Ok(db, append((S|1,2,3), (S|4,5), X), X, (S| (S|1,2,3,4,5) ));
        Ok(db, append((S|1,2,3), Nil, X), X, (S| (S|1,2,3) ));
        Ok(db, append(Nil, Nil, Nil));
        Ok(db, append(Nil, Nil, X), X, (S| Nil ));
        Ok(db, append((S|1,2,3), X, (S|1,2,3,4,5)), X, (S| (S|4,5)));

        Ok(db, nth(1, (S|10), 10));
        Ok(db, nth(1, (S|1,2,3), 1));
        Ok(db, nth(2, (S|1,2,3), 2));
        Ok(db, nth(3, (S|1,2,3), 3));
        Ok(db, nth(1, (S|10,20,30), X), X, (S|10));
        Ok(db, nth(2, (S|10,20,30), X), X, (S|20));
        Ok(db, nth(3, (S|10,20,30), X), X, (S|30));
        Fail(db, nth(4, (S|1,2,3)));
        Fail(db, nth(0, (S|1,2,3)));
        Fail(db, nth(-1, (S|1,2,3)));

        Ok(db, nth0(0, (S|10), 10));
        Ok(db, nth0(0, (S|1,2,3), 1));
        Ok(db, nth0(1, (S|1,2,3), 2));
        Ok(db, nth0(2, (S|1,2,3), 3));
        Fail(db, nth0(3, (S|1,2,3)));
        Fail(db, nth0(-1, (S|1,2,3)));

        Ok(db, permutation((S|1,2), X), X, (S| (S|1,2), (S|2,1)));
        Ok(db, permutation((S|1,2,3), (S|3,1,2)));
        Fail(db, permutation((S|1,2,3), (S|1,2)));
        Fail(db, permutation((S|1,2,3), (S|1,1,2)));

        Ok(db, select(2, (S|1,2,3), (S|1,3)));
        Ok(db, select(X, (S|1,2,3), (S|1,2)), X, (S|3));
        Ok(db, select(1, (S|X,2,3), (S|2,3)), X, (S|1));

        Ok(db, reverse((S|1,2,3), (S|3,2,1)));
        Ok(db, reverse((S|1,2,3), X), X, (S|(S|3,2,1)));
        Ok(db, reverse(Nil, Nil));
        Ok(db, reverse((S|X,2,3), (S|3,2,1)), X, (S|1));

        Ok(db, length((S|1,2,3), 3));
        Ok(db, length(Nil, 0));
        Ok(db, length((S|X,Y,Z), 3));

        //Hooks::EnableAll();
        PlgAtom f("f"), g("g");
        Ok(db, assert(f(1)) & f(X), X, (S|1));
        Ok(db, asserta(f(2)) & f(X), X, (S|2,1));
        Ok(db, assertz(f(3)) & f(X), X, (S|2,1,3));
        Fail(db, f(4));

        Ok(db, atom(f));
        Fail(db, atom(X));
        Fail(db, atom(f(X)));
        Fail(db, atom(1));
        Fail(db, atom(S|1,2,3));

        Ok(db, atomic(f));
        Ok(db, atomic(1));
        Fail(db, atomic(f(X)));
        Fail(db, atomic(X));
        Fail(db, atomic(S|1,2,3));

        Ok(db, compound(f(X)));
        Fail(db, compound(f));
        Fail(db, compound(1));
        Fail(db, compound(X));
        Fail(db, compound((S|1,2,3)));

        Ok(db, nonvar(f(X)));
        Ok(db, nonvar(1));
        Ok(db, nonvar(f));
        Ok(db, nonvar((S|1,2,3)));
        Fail(db, nonvar(X));

        Ok(db, var(X));
        Fail(db, var(f(X)));
        Fail(db, var(1));
        Fail(db, var(f));
        Fail(db, var((S|1,2,3)));

        Ok(db, univ(f(g), (S|f,g)));
        Ok(db, univ(f(1,2,3), X), X, (S| (S|f,1,2,3) ));
        Fail(db, univ(f(1), (S|f,2)));

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
