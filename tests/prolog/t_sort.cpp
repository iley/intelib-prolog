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
    using PlgStdLib::truth;
    try {
        SListConstructor S;
        PlgAtom qsort("qsort"), append("my_append"), split("split"), select("my_select"), psort("psort"), perm("my_perm"), sorted("sorted");
        PlgVariable H("H"), T("T"), X("X"), L("L"), R("R"), LS("LS"), RS("RS"), Res("Res");

        // standard library functions
        append(Nil, X, X) <<= truth;
        append(H^T, L, H^R) <<= append(T, L, R);

        select(X, X^T, T) <<= truth;
        select(X, H^T, H^R) <<= select(X, T, R);

        // quick sort
        qsort(Nil, Nil) <<= truth;
        qsort(H^T, Res) <<= split(H, T, L, R) & qsort(L, LS) & qsort(R, RS) & append(LS, H ^ RS, Res);

        split(X, Nil, Nil, Nil) <<= truth;
        split(X, H^T, H^LS, RS) <<= (H <= X) & split(X, T, LS, RS);
        split(X, H^T, LS, H^RS) <<= (H >  X) & split(X, T, LS, RS);

        // permutation sort
        psort(L,R) <<= perm(L,R) & sorted(R);

        sorted(Nil) <<= truth;
        sorted((S|X)) <<= truth;
        sorted(X^(H^T)) <<= (X <= H) & sorted(H^T);

        perm(Nil, Nil) <<= truth;
        perm(H^T, R) <<= perm(T,L) & select(H,R,L);

        // test all sorting algorithms
        for (SReference p = (S|qsort, psort); !p.IsEmptyList(); p = p.Cdr()) {
            PlgAtom sort = p.Car();
            TestSection(sort->TextRepresentation().c_str());

            Ok(sort((S|3,1,2), X), X, (S| (S|1,2,3) ));
            Ok(sort((S|3,2,1), X), X, (S| (S|1,2,3) ));
            Ok(sort((S|5,1,2,4), X), X, (S| (S|1,2,4,5) ));
            Ok(sort((S|1), X), X, (S| (S|1) ));
            Ok(sort(Nil, X), X, (S| Nil ));
            Ok(sort((S|1,2,3), X), X, (S| (S|1,2,3) ));

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
