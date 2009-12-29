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
#include "sexpress/svector.hpp"

#include "tools/sstream.hpp"
#include "genlisp/conteval.hpp"
#include "genlisp/dumpcont.hpp"

#include "scheme/scheme.hpp"
#include "scheme/schfun.hpp"
#include "scheme/schsymb.hpp"

#include "sch_std.hpp"
#include "sch_sel.hpp"


void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}


SchListConstructor L;

SchSymbol CONS("CONS", SReference(new SchFunctionCons));
SchSymbol CAR("CAR", SReference(new SchFunctionCar));
SchSymbol DEFINE("DEFINE", SReference(new SchFunctionDefine));
SchSymbol LET("LET", SReference(new SchFunctionLet));
SchSymbol SETQ("SET!", SReference(new SchFunctionSetq));
SchSymbol SETF("SETF!", SReference(new SchFunctionSetf));

int main()
{
    poc();
    try {
        TestSection("Standard Functions");
        TestSubsection("CONS");
        {
            SchReference res = (L|CONS, 1, 2).Evaluate();
            TESTTR("cons", res, "(1 . 2)");
        }
        TestSubsection("CONS_superposition");
        {
            SchReference res = (L|CONS, (L|CONS, 7, 77),
                                        (L|CONS, 8, 88)).Evaluate();
            TESTTR("cons_superposition", res, "((7 . 77) 8 . 88)");
        }
        TestSubsection("QUOTE");
        {
            SchSymbol A("A", 25);
            SchReference res = (L|CONS, A, ~A).Evaluate();
            TESTTR("quote", res, "(25 . A)");
        }
        TestSubsection("DEFINE");
        {
            SchSymbol A("A");
            SchReference res = (L|DEFINE, A, 252).Evaluate();
            TESTTR("top_level_define", A->GetGlobalValue(), "252");
        }
        TestSubsection("LexicShadowing");
        {
            SchSymbol A("A");
            SchSymbol B("B");
            SchSymbol C("C");
            SchemeContinuation cont;
            (L|DEFINE, A, "global").Evaluate(cont);
            (L|DEFINE, B, "error_unchanged").Evaluate(cont);
            (L|DEFINE, C, "error_unchanged").Evaluate(cont);
            SReference res =
                (L|LET, (L| (L| A, "lex")),
                    (L|LET, (L| (L| A, "inner")),
                         (L|SETQ, B, A)
                    ),
                    (L|SETQ, C, A)
                ).Evaluate(cont);
            TESTTR("lexic_shadow_inner", SchReference(B).Evaluate(cont),
                                           "\"inner\"");
            TESTTR("lexic_shadowed",     SchReference(C).Evaluate(cont),
                                           "\"lex\"");
            TESTTR("lexic_shadow_glob",  SchReference(A).Evaluate(cont),
                                           "\"global\"");
        }
        TestSubsection("SETF_bug");
        {
            SchSymbol A("A");
            SchemeContinuation cont;
            SStreamStdout Stdout;
            (L|SETQ, A, ~(L|1, 2)).Evaluate(cont);
            cont.PushTodo(IntelibContinuation::just_evaluate,
                          (L|SETF, (L| CAR, A), 111));
            while(!cont.Ready()) {
                cont.Step();
                printf("--------------------------------------------\n");
                IntelibDumpContinuation(cont, Stdout.GetPtr());
            }
            TESTTR("setf_bug_fixed", SchReference(A).Evaluate(cont),"(111 2)");


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
