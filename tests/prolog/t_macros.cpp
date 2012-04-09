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
        PlgVariable X("X"), Head("Head"), Body("Body");
        PlgAtom i_say("i_say"), human("human"), mortal("mortal"), socrates("socrates");

        TestSection("term_expansion");

        term_expansion(i_say(Head, Body), (Head << Body)) <<= truth ;
        term_expansion(i_say(Head), (Head << truth)) <<= truth;
        i_say(mortal(X), human(X)) <<= truth;
        i_say(human(socrates)) <<= truth;

        Ok(mortal(X), X, (S|socrates));

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
