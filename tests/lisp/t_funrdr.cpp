//   InteLib                                    http://www.intelib.org
//   The file tests/lisp/t_funrdr.cpp
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
#include <stdlib.h>
#include <unistd.h>
#include "tests.hpp"

#include "sexpress/iexcept.hpp"
#include "sexpress/sstring.hpp"
#include "lisp/lisp.hpp"
#include "lisp/lpackage.hpp"
#include "lisp/lsymbol.hpp"
#include "lfun_std.hpp"
#include "lfun_sel.hpp"
#include "lfun_io.hpp"
#include "lfun_rdr.hpp"

void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}

LListConstructor L;

LFunctionalSymbol<LFunctionCons> CONS("CONS");
LFunctionalSymbol<LFunctionSetq> SETQ("SETQ");
LFunctionalSymbol<LFunctionCar> CAR("CAR");
LFunctionalSymbol<LFunctionCdr> CDR("CDR");
LFunctionalSymbol<LFunctionCddar> CDDAR("CDDAR");
LFunctionalSymbol<LFunctionFirst> FIRST("FIRST");
LFunctionalSymbol<LFunctionThird> THIRD("THIRD");
LFunctionalSymbol<LFunctionList> LIST("LIST");
LFunctionalSymbol<LFunctionDefun> DEFUN("DEFUN");
LFunctionalSymbol<LFunctionDefmacro> DEFMACRO("DEFMACRO");
LFunctionalSymbol<LFunctionAppend> APPEND("APPEND");
LFunctionalSymbol<LFunctionNot> NOT("NOT");
LFunctionalSymbol<LFunctionNull> lNULL("lNULL");
LFunctionalSymbol<LFunctionAnd> AND("AND");
LFunctionalSymbol<LFunctionOr> OR("OR");
LFunctionalSymbol<LFunctionCond> COND("COND");
LFunctionalSymbol<LFunctionIf> IF("IF");
LFunctionalSymbol<LFunctionPlus> PLUS("PLUS");
LFunctionalSymbol<LFunctionDifference> DIFFERENCE("DIFFERENCE");
LFunctionalSymbol<LFunctionTimes> TIMES("TIMES");
LFunctionalSymbol<LFunctionQuotient> QUOTIENT("QUOTIENT");
LFunctionalSymbol<LFunctionGreaterp> GREATERP("GREATERP");
LFunctionalSymbol<LFunctionLessp> LESSP("LESSP");
LFunctionalSymbol<LFunctionMathequal> MATHEQUAL("MATHEQUAL");
LFunctionalSymbol<LFunctionAtom> ATOM("ATOM");
LFunctionalSymbol<LFunctionNumberp> NUMBERP("NUMBERP");
LFunctionalSymbol<LFunctionSymbolp> SYMBOLP("SYMBOLP");
LFunctionalSymbol<LFunctionListp> LISTP("LISTP");
LFunctionalSymbol<LFunctionEq> EQ("EQ");
LFunctionalSymbol<LFunctionEql> EQL("EQL");
LFunctionalSymbol<LFunctionEqual> EQUAL("EQUAL");
LFunctionalSymbol<LFunctionLast> LAST("LAST");
LFunctionalSymbol<LFunctionMemberp> MEMBERP("MEMBERP");
LFunctionalSymbol<LFunctionMapcar> MAPCAR("MAPCAR");
LFunctionalSymbol<LFunctionLet> LET("LET");

LFunctionalSymbol<LFunctionSaferead> SAFEREAD("SAFEREAD");


FILE* incoming_stream(const char *s)
{
    int fd[2];
    pipe(fd);
    if(fork()==0) {
        close(fd[0]);
        FILE* f = fdopen(fd[1], "w");
        while(*s) fputc(*s++, f);
        fclose(f);
        exit(0);
    } else {
        close(fd[1]);
        return fdopen(fd[0], "r");
    }
}


int main()
{
    poc();
    try {
        TestSection("Lisp Reader Functions");
        TestSubsection("package");
        {
            FILE* f = incoming_stream("(A B) (C A D)");
            LReference lstream = new SExpressionStreamFile(f);
            LReference pkg = new LExpressionPackage;
            LReference res1 = (L|SAFEREAD, ~lstream, 11, 22, ~pkg).Evaluate();
            LReference res2 = (L|SAFEREAD, ~lstream, 33, 44, ~pkg).Evaluate();

            fclose(f);

            TESTTR("read_1", res1, "(A B)");
            TESTTR("read_2", res2, "(C A D)");

            TESTB("package_works", res1.Car().GetPtr() ==
                 res2.Cdr().Car().GetPtr());

            LReference res3 =
                (L|SAFEREAD, "(E F A G)", 0, 0, ~pkg).Evaluate();

            TESTTR("saferead_reads_from_string", res3, "(E F A G)");
            TESTB("package_still_works", res1.Car().GetPtr() ==
                 res3.Cdr().Cdr().Car().GetPtr());

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


