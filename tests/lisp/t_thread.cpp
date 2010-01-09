//   InteLib                                    http://www.intelib.org
//   The file tests/lisp/t_thread.cpp
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


