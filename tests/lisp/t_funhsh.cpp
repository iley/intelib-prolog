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
#include "sexpress/shashtbl.hpp"
#include "lisp/lsymbol.hpp"
#include "lisp/lcont.hpp"
#include "lisp/llambda.hpp"
#include "lfun_std.hpp"
#include "lfun_sel.hpp"
#include "lfun_io.hpp"
#include "lfun_hsh.hpp"

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


LFunctionalSymbol<LFunctionMaphash> MAPHASH("MAPHASH");



LReference LTrue(*PTheLispBooleanTrue);
LReference LFalse(*PTheLispBooleanFalse);

int main()
{
    poc();
    try {
        TestSection("Hashtable Functions");
        TestSubsection("MAPHASH");
        {
            LSymbol A("A");
            LSymbol B("B");
            LSymbol H("H");
            LSymbol C("C");
            LSymbol D("D");
            LSymbol E("E");
            C->SetDynamicValue(0);
            SExpressionHashTable *ph;
            H->SetDynamicValue(LReference((ph = new SExpressionHashTable)));
            ph->AddItem(1,   1010);
            ph->AddItem(2,  10010);
            ph->AddItem(3, 100010);
            (L|MAPHASH,
             (L|FUNCTION, (L|LAMBDA, (L|A, B),
                           (L|SETQ, C, (L|PLUS, C, A, B)))),
             H).Evaluate();
            TESTTR("maphash", LReference(C).Evaluate(), "111036");
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


