//   InteLib                                    http://www.intelib.org
//   The file tests/lisp/t_funm.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include <stdio.h>
#include "tests.hpp"

#include "sexpress/iexcept.hpp"
#include "sexpress/sstring.hpp"

#include "lisp/lisp.hpp"
#include "lisp/lsymbol.hpp"

#include "lfun_m.hpp"


void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}


LListConstructor L;

LFunctionalSymbol<LFunctionSin> SIN("SIN");


int main()
{
    poc();
    try {
        TestSection("Math Functions");
        TestSubsection("SIN");
        {
            LReference res = (L|SIN, 0.0).Evaluate();
            TESTB("sin", res.GetFloat() < 0.001 && res.GetFloat() > -0.001);
        }
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
    poc();
    return 0;
}


