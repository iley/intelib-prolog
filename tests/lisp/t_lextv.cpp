//   InteLib                                    http://www.intelib.org
//   The file tests/lisp/t_lextv.cpp
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
#include "lisp/lextvar.hpp"
#include "lisp/lsymbol.hpp"
#include "lfun_std.hpp"


LListConstructor L;
LFunctionConstructor F;

LFunctionalSymbol<LFunctionPlus> PLUS("PLUS");
LFunctionalSymbol<LFunctionSetf> SETF("SETF");

int main()
{
    try {
        TestSection("IndirectVariables");
        TestSubsection("Int");
        {
            int i = 55;
            LReference r = (L|PLUS, F&i, 5);
            LReference res = r.Evaluate();
            TESTTR("first_value", res, "60");
            i = 75;
            res = r.Evaluate();
            TESTTR("changed_value", res, "80");
            (L|SETF, F&i, 25).Evaluate();
            TEST("indirect_setf", i, 25);
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
    return 0;
}


