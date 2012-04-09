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
SReference &Nil = *(GetEmptyList());

int main()
{
    try {
        using namespace PlgStdLib;

        SListConstructor S;
        PlgVariable X("X"), Y("Y"), Z("Z");

        TestSection("Standard Library");

        Ok(member(1, (S|1)));
        Ok(member(1, (S|1,2,3)));
        Ok(member(2, (S|1,2,3)));
        Ok(member(3, (S|1,2,3)));
        Fail(member(4, (S|1,2,3)));
        Fail(member(1, Nil));

        Fail(nope(member(1, (S|1))));
        Fail(nope(member(1, (S|1,2,3))));
        Fail(nope(member(2, (S|1,2,3))));
        Fail(nope(member(3, (S|1,2,3))));
        Ok(nope(member(4, (S|1,2,3))));
        Ok(nope(member(1, Nil)));

        Ok(append((S|1,2,3), (S|4,5), (S|1,2,3,4,5)));
        Ok(append((S|1,2,3), (S|4,5), X), X, (S| (S|1,2,3,4,5) ));
        Ok(append((S|1,2,3), Nil, X), X, (S| (S|1,2,3) ));
        Ok(append(Nil, Nil, Nil));
        Ok(append(Nil, Nil, X), X, (S| Nil ));
        Ok(append((S|1,2,3), X, (S|1,2,3,4,5)), X, (S| (S|4,5)));

        Ok(nth(1, (S|10), 10));
        Ok(nth(1, (S|1,2,3), 1));
        Ok(nth(2, (S|1,2,3), 2));
        Ok(nth(3, (S|1,2,3), 3));
        Ok(nth(1, (S|10,20,30), X), X, (S|10));
        Ok(nth(2, (S|10,20,30), X), X, (S|20));
        Ok(nth(3, (S|10,20,30), X), X, (S|30));
        Fail(nth(4, (S|1,2,3)));
        Fail(nth(0, (S|1,2,3)));
        Fail(nth(-1, (S|1,2,3)));

        Ok(nth0(0, (S|10), 10));
        Ok(nth0(0, (S|1,2,3), 1));
        Ok(nth0(1, (S|1,2,3), 2));
        Ok(nth0(2, (S|1,2,3), 3));
        Fail(nth0(3, (S|1,2,3)));
        Fail(nth0(-1, (S|1,2,3)));

        Ok(permutation((S|1,2), X), X, (S| (S|1,2), (S|2,1)));
        Ok(permutation((S|1,2,3), (S|3,1,2)));
        Fail(permutation((S|1,2,3), (S|1,2)));
        Fail(permutation((S|1,2,3), (S|1,1,2)));

        Ok(select(2, (S|1,2,3), (S|1,3)));
        Ok(select(X, (S|1,2,3), (S|1,2)), X, (S|3));
        Ok(select(1, (S|X,2,3), (S|2,3)), X, (S|1));

        Ok(reverse((S|1,2,3), (S|3,2,1)));
        Ok(reverse((S|1,2,3), X), X, (S|(S|3,2,1)));
        Ok(reverse(Nil, Nil));
        Ok(reverse((S|X,2,3), (S|3,2,1)), X, (S|1));

        Ok(length((S|1,2,3), 3));
        Ok(length(Nil, 0));
        Ok(length((S|X,Y,Z), 3));

        PlgAtom f("f"), g("g");
        Ok(assert(f(1)) & f(X), X, (S|1));
        Ok(asserta(f(2)) & f(X), X, (S|2,1));
        Ok(assertz(f(3)) & f(X), X, (S|2,1,3));
        Fail(f(4));

        //FIXME
        //Ok(retract(f(2)));
        //Ok(f(X), X, (S|1,3));
        //Fail(retract(f(100)));
        //Ok(f(X), X, (S|1,3));
        //Ok(retract(f(X)));
        //Fail(f(X));

        Ok(atom(f));
        Fail(atom(X));
        Fail(atom(f(X)));
        Fail(atom(1));
        Fail(atom(S|1,2,3));

        Ok(atomic(f));
        Ok(atomic(1));
        Fail(atomic(f(X)));
        Fail(atomic(X));
        Fail(atomic(S|1,2,3));

        Ok(compound(f(X)));
        Fail(compound(f));
        Fail(compound(1));
        Fail(compound(X));
        Fail(compound((S|1,2,3)));

        Ok(nonvar(f(X)));
        Ok(nonvar(1));
        Ok(nonvar(f));
        Ok(nonvar((S|1,2,3)));
        Fail(nonvar(X));

        Ok(var(X));
        Fail(var(f(X)));
        Fail(var(1));
        Fail(var(f));
        Fail(var((S|1,2,3)));

        Ok(univ(f(g), (S|f,g)));
        Ok(univ(f(1,2,3), X), X, (S| (S|f,1,2,3) ));
        Fail(univ(f(1), (S|f,2)));

        //Ok(dcg_translate_rule(f >>= g, X) & (X ^= (f(X,Y) <<= g(X,Y))));
        Ok(dcg_translate_rule(f >>= g, X) );

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
