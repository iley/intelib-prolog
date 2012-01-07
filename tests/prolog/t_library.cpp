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

        //PlgGlobalHooks.Call = callTraceHook;
        //PlgGlobalHooks.Unify = unifyTraceHook;

        SListConstructor S;
        PlgDatabase db;

        TestSection("Standard Library");

        ok(db, member(1, (S|1)));
        ok(db, member(1, (S|1,2,3)));
        ok(db, member(2, (S|1,2,3)));
        ok(db, member(3, (S|1,2,3)));
        fail(db, member(4, (S|1,2,3)));
        fail(db, member(1, Nil));

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
