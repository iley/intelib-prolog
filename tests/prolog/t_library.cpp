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

        //PlgGlobalHoOks.Call = callTraceHoOk;
        //PlgGlobalHoOks.Unify = unifyTraceHoOk;

        SListConstructor S;
        PlgDatabase db;
        PlgVariableName X("X");

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
