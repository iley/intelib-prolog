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

#include "sexpress/sstring.hpp"
#include "lisp/lisp.hpp"
#include "lisp/lsymbol.hpp"
#include "lisp/llambda.hpp"
#include "lfun_std.hpp"
#include "lfun_sel.hpp"

LFunctionalSymbol<LFunctionSetq> SETQ("SETQ");
LFunctionalSymbol<LFunctionList> LIST("LIST");
LFunctionalSymbol<LFunctionDefun> DEFUN("DEFUN");
LFunctionalSymbol<LFunctionThird> THIRD("THIRD");
LFunctionalSymbol<LFunctionCond> COND("COND");




LListConstructor L;

int main()
{
    try {
        TestSection("Threads");
        TestSubsection("TailReturnReference bug");
        {
            LSymbol F("F");
            LSymbol G("G");
            LSymbol A("A");
            (L|DEFUN, F, (L|A),
             (L|SETQ, A, (L|LIST, A, A)),
             A
            ).Evaluate();
            LReference res = (L|F, 22).Evaluate();
            TEST("tail_return_reference_bug_fixed",
                 res->TextRepresentation().c_str(), "(22 22)");
        }
        {
            LSymbol F("F");
            LSymbol A("A");
            (L|DEFUN, F, (L|A),
             //          (L|THIRD, (L|COND, (L|T, (L|LIST, A, 1, 2))))
             (L|THIRD, (L|LIST, 2, 1, A))
            ).Evaluate();
            LReference res = (L|F, 33).Evaluate();
            TEST("tail_return_reference_bug_fixed2",
                 res->TextRepresentation().c_str(), "33");
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


