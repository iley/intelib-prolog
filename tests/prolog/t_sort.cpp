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
        TestSection("Quick Sort");
        {
            SListConstructor S;
            PlgAtom qsort("qsort"), append("append"), split("split");
            PlgVariableName H("H"), T("T"), X("X"), L("L"), R("R"), LS("LS"), RS("RS");

            PlgDatabase db;

            db.Add( qsort(Nil, Nil) );
            db.Add( qsort(H^T, R) <<= split(H, T, L, R) & qsort(L, LS) & qsort(R, RS) & append(LS, H ^ RS, R) );

            db.Add( split(X, Nil, Nil, Nil) );
            db.Add( split(X, H^T, H^LS, RS) <<= (H <= X) & split(X, T, LS, RS) );
            db.Add( split(X, H^T, LS, H^RS) <<= (H >  X) & split(X, T, LS, RS) );

            db.Add( append(Nil, X, X) );
            db.Add( append(H^T, L, H^R) <<= append(T, L, R) );

            ok(db, qsort((S|3,1,2), X), X, (S| (S|1,2,3) ));

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
