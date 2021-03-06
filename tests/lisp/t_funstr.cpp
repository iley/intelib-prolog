//   InteLib                                    http://www.intelib.org
//   The file tests/lisp/t_funstr.cpp
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
#include "lisp/lcont.hpp"
#include "lisp/llambda.hpp"

#include "lfun_std.hpp"
#include "lfun_sel.hpp"
#include "lfun_str.hpp"

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


LFunctionalSymbol<LFunctionList2string> LIST2STRING("LIST2STRING");
LFunctionalSymbol<LFunctionString2c> STRING2C("STRING2C");

LFunctionalSymbol<LFunctionCharacterp> CHARACTERP("CHARACTERP");



LReference LTrue(*PTheLispBooleanTrue);
LReference LFalse(*PTheLispBooleanFalse);

int main()
{
    try {
        TestSection("String manipulation functions");
        TestSubsection("List2string");
        {
            LReference form;

            form = ((L|LIST2STRING, ~(L)));
            TESTTR("list2string_empty", form.Evaluate(), "\"\"");
            form = ((L|LIST2STRING, ~(L|"A")));
            TESTTR("list2string_1", form.Evaluate(), "\"A\"");
            form = ((L|LIST2STRING, ~(L|"A", "B", "C", "D", "E", "F")));
            TESTTR("list2string_several", form.Evaluate(), "\"ABCDEF\"");

        }
        TestSubsection("String2c");
        {
            TEST("string2c_empty", (L|STRING2C, "").Evaluate().GetString(),
                 "\"\"");
            TEST("string2c_single", (L|STRING2C, "a").Evaluate().GetString(),
                 "\"a\"");
            TEST("string2c_space", (L|STRING2C, " ").Evaluate().GetString(),
                 "\" \"");
            TEST("string2c_newline", (L|STRING2C, "\n").Evaluate().GetString(),
                 "\"\\n\"");
            TEST("string2c", (L|STRING2C, "\n\tABC \007.").
                                              Evaluate().GetString(),
                 "\"\\n\\tABC \\007.\"");
        }
        TestSubsection("Characterp");
        {
            TESTB("characterp_ok", (L|CHARACTERP, 'a').Evaluate().IsTrue());
            TESTB("characterp_1str_false",
                                 !(L|CHARACTERP, "a").Evaluate().IsTrue());
            TESTB("characterp_2_false",
                                 !(L|CHARACTERP, "ab").Evaluate().IsTrue());
            TESTB("characterp_0_false",
                                 !(L|CHARACTERP, "").Evaluate().IsTrue());
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


