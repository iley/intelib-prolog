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


