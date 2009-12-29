// +-------------------------------------------------------------------------+
// |               I__n__t__e__L__i__b           0.6.21 development          |
// | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
// |                                                                         |
// | This is free software.  See GNU GENERAL PUBLIC LICENSE v.2 for details. |
// | GNU GPL v2  is  found  in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
// |     Please see also docs/readme.txt and visit http://www.intelib.org    |
// |                                                                         |
// | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
// +-------------------------------------------------------------------------+




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


