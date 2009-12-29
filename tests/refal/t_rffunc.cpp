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

#include "lisp/lterm.hpp"
#include "lisp/lsymbol.hpp"
#include "lisp/lstring.hpp"
#include "refal/rexpress.hpp"
#include "refal/rlists.hpp"
#include "refal/rvars.hpp"
#include "refal/rsubcls.hpp"
#include "refal/rpattern.hpp"
#include "refal/rexcept.hpp"
#include "refal/rfuncts.hpp"
#include "refal/rfun_std.hpp"


void doprint(const LTerm *ref)
{
    printf("%s\n", ref ? ref->TextRepresentation().c_str() : "(unbound)");
}

void doprint(const LReference &ref)
{
    doprint(ref.GetPtr());
}

void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", LTerm::object_counter);
#endif
}

void printstructure(LTerm *ref)
{
    printf("{%lx:", (unsigned long)ref);
    LDotPair *dp = IntelibDynamicCastP<LDotPair>(ref);
    if(dp) {
        printf("(");
        printstructure(dp->Car().GetPtr());
        printf(".");
        printstructure(dp->Cdr().GetPtr());
        printf(")");
    } else {
        RfCall *rc = IntelibDynamicCastP<RfCall>(ref);
        if(rc) {
            printf("<");
            printstructure(rc->GetList().GetPtr());
            printf(">");
        } else {
            doprint(ref);
        }
    }
    printf("}");
}



int anchor_counter;
void anchor() {
    anchor_counter ++;
}

#define TestTxRep(name, res, pattern) \
  Test(name, res.GetPtr() ? res->TextRepresentation().c_str() : 0, pattern)

LListConstructor L;
RfListConstructor R;
RfFunctionConstructor RFUNC;

int main()
{
    poc();
    try {
        LSymbol fun("fun");
        LFunctionalSymbol<LFunctionRefalcall> REFALCALL("REFALCALL");
        poc();
        TestSection("RefalFunctions");
        TestSubsection("Palindrom");
        {
            RfVariable_S s_1("s.1");
            RfVariable_E e_1("e.1"), e_2("e.2");
            RFUNC(fun) [(L)                ^ (L| "True")]
            [(L| s_1)           ^ (L| "True")]
            [(L| s_1, e_2, s_1) ^ (L| (R| fun, e_2))]
            [(L| e_1)           ^ (L| "False")] ;
            TestTxRep("fun_palindrom_constructed", fun->GetFunction(),
                      "#<REFAL FUNCTION("
                      "(#<RFPATTERN [][][][][]> \"T\" \"r\" \"u\" \"e\") "
                      "(#<RFPATTERN [s.1 ][][][][]> \"T\" \"r\" \"u\" \"e\") "
                      "(#<RFPATTERN [s.1 ][][e.2 ][s.1 ][]> <fun e.2>) "
                      "(#<RFPATTERN [][][e.1 ][][]> \"F\" \"a\" \"l\" \"s\" \"e\")"
                      ")>");
            //      RfFunction *funptr =
            //         static_cast<RfFunction*>(fun->GetFunction().GetPtr());

            LReference param;

            param = (L|"a");
            LTheDefaultThread.ResetReturn();
            fun.Apply(param, LTheDefaultThread);
            TestTxRep("palindrom_single", LTheDefaultThread.GetReturnBuffer(),
                      "(\"T\" \"r\" \"u\" \"e\")");

#if 0
            param = (L|"a", "b", "c", "d", "e", "d", "c", "b", "a");
            LTheDefaultThread.ResetReturn();
            fun.Apply(param, LTheDefaultThread);
            TestTxRep("palindrom_odd", LTheDefaultThread.GetReturnBuffer(),
                      "(\"T\" \"r\" \"u\" \"e\")");

            param = RfReference((L|"abcddcba"));
            LTheDefaultThread.ResetReturn();
            fun.Apply(param, LTheDefaultThread);
            TestTxRep("palindrom_even", LTheDefaultThread.GetReturnBuffer(),
                      "(\"T\" \"r\" \"u\" \"e\")");

            param = RfReference((L|"abcdefdcba"));
            LTheDefaultThread.ResetReturn();
            fun.Apply(param, LTheDefaultThread);
            TestTxRep("palindrom_false", LTheDefaultThread.GetReturnBuffer(),
                      "(\"F\" \"a\" \"l\" \"s\" \"e\")");
#endif // 0  

            param = LTheEmptyList;
            LTheDefaultThread.ResetReturn();
            fun.Apply(param, LTheDefaultThread);
            TestTxRep("palindrom_empty", LTheDefaultThread.GetReturnBuffer(),
                      "(\"T\" \"r\" \"u\" \"e\")");

        }
        poc();
        TestSubsection("RefalCall");
        {
            LReference ref((L|REFALCALL, "abc", (R|fun, "abcdcba"),
                            "abc"));
            LReference res = ref.Evaluate();

            TestTxRep("refal_call", res, "(\"abcTrueabc\")");
        }
        TestSubsection("RefalCall2");
        {
            LReference ref((L|REFALCALL, (R|fun, "abcdcba"),
                            (R|fun, "abc"), "abc"));
            LReference res = ref.Evaluate();

            TestTxRep("refal_call", res, "(\"TrueFalseabc\")");
        }
        poc();
        TestSubsection("TreesAtRight");
        {
            LSymbol f2("f2");
            LSymbol G("G");
            RfVariable_S s_1("s.1");
            RfVariable_E e_1("e.1"), e_2("e.2");
            RFUNC(f2)
            [(L|e_1) ^ (L|(L|e_1, (L|e_1, (L|e_1), e_1), e_1, (L|e_1)))];
            LReference ref((L|REFALCALL, (R|f2, G)));
            LReference res = ref.Evaluate();
            TestTxRep("tree", res, "((G (G (G) G) G (G)))");
            ref = (L|REFALCALL, (R|f2, G, "abc"));
            res = ref.Evaluate();
            TestTxRep("tree2", res,
                      "((G \"abc\" (G \"abc\" (G \"abc\") G \"abc\")"
                      " G \"abc\" (G \"abc\")))");

        }
        poc();
        TestSubsection("CallsAtRight");
        {
            LSymbol G("G");
            RfVariable_S s_1("s.1");
            RfVariable_E e_1("e.1"), e_2("e.2");
            LReference ref, res;
            LSymbol f3("f2");
            RFUNC(f3)
            [(L|s_1, e_1) ^ (L|(L|s_1, e_1), (R|f3, e_1))]
            [(L)          ^ (L)];
            ref = (L|REFALCALL, (R|f3, G, G, G, G));
            res = ref.Evaluate();
            TestTxRep("regression", res, "((G G G G) (G G G) (G G) (G))");

            LSymbol f4("f2");
            RFUNC(f4)
            [(L|s_1, e_1) ^ (L|e_1, (R|f4, e_1), (L|s_1, e_1))]
            [(L)          ^ (L)];

            ref = (L|REFALCALL, (R|f4));
            res = ref.Evaluate();
            Test("regression2_0", res->IsEmptyList());

            ref = (L|REFALCALL, (R|f4, G));
            res = ref.Evaluate();
            TestTxRep("regression2_1", res, "((G))");

            ref = (L|REFALCALL, (R|f4, G, G));
            res = ref.Evaluate();
            TestTxRep("regression2_2", res, "(G (G) (G G))");

            ref = (L|REFALCALL, (R|f4, G, G, G));
            res = ref.Evaluate();
            TestTxRep("regression2_3", res, "(G G G (G) (G G) (G G G))");

            ref = (L|REFALCALL, (R|f4, G, G, G, G));
            res = ref.Evaluate();
            TestTxRep("regression2_4", res,
                      "(G G G G G G (G) (G G) (G G G) (G G G G))");
        }
        poc();
        TestSubsection("WHERE");
        {
            LSymbol ReadWord("ReadWord");
            RfVariable_E e_1("e.1");
            RfVariable_S s_1("s.1");
            RfVariable_E e_10("e.10");
            RfVariable_E e_20("e.20");
            RfVariable_E e_2("e.2");
            RfVariable_S s_2("s.2");
            RfVariable_E e_3("e.3");

            RFUNC(ReadWord)
            [(L|(L|e_1), s_2, e_3,
              WHERE((L|"{}()<>=;,:\' \t\n" ),(L|e_10, s_2, e_20)))
             ^ (L|(L|e_1), (R|ReadWord, (L), e_3))]
            [(L|(L|e_1), s_2, e_3)      ^ (L|(R|ReadWord, (L|e_1, s_2), e_3))]
            [(L|(L|e_1))        ^ (L|(L|e_1))];

            doprint(ReadWord->GetFunction());
            LReference ref, res;

            ref = (L|REFALCALL, (R|ReadWord, (L), "abcd{efgh"));
            res = ref.Evaluate();
            TestTxRep("readword", res, "((\"abcd\") (\"efgh\"))");

            ref = (L|REFALCALL, (R|ReadWord, (L), "abcd"));
            res = ref.Evaluate();
            TestTxRep("readword2", res, "((\"abcd\"))");

            ref = (L|REFALCALL, (R|ReadWord, (L), "abcd{ }efgh"));
            res = ref.Evaluate();
            LReference exp_res = (L|(L|"abcd"),(L),(L),(L|"efgh"));
            TestTxRep("readword3", res, exp_res->TextRepresentation().c_str());

            ref = (L|REFALCALL, (R|ReadWord, (L), "abcd efgh"));
            res = ref.Evaluate();
            TestTxRep("readword4", res, "((\"abcd\") (\"efgh\"))");

        }
        TestSubsection("CallSequenceBug");
        {
            LSymbol f1("f1");
            LSymbol f2("f2");
            LSymbol f3("f3");
            LSymbol f4("f4");
            RfVariable_E e_1("e.1");
            RfVariable_S s_1("s.1");
            RfVariable_E e_10("e.10");
            RfVariable_E e_20("e.20");
            RfVariable_E e_2("e.2");
            RfVariable_S s_2("s.2");
            RfVariable_E e_3("e.3");


            RFUNC(f1)
            [(L|e_1) ^ (L|1)];
            RFUNC(f2)
            [(L|e_1) ^ (L|e_1)];
            RFUNC(f3)
            [(L|e_1) ^ (L|3, e_1)];

            LReference ref, res;

            // <f3 (((<f3 <f1>>) <f2 22>))>
            ref = (L|REFALCALL, (R|f3, (L|
                                        (L|
                                         (L|
                                          (R|f3,
                                           (R|f1)
                                          )
                                         ),
                                         (R|f2, 22)
                                        )
                                       )
                                )
                  );
            res = ref.Evaluate();
            TestTxRep("didnt_skip", res, "(3 (((3 1) 22)))");

            // <f3 ((100)(200)<f1>)<f2, 22>)
            ref = (L|REFALCALL, (R|f3, (L|
                                        (L|100),(L|200),(R|f1)
                                       ),
                                 (R|f2, 22)
                                )
                  );
            res = ref.Evaluate();
            TestTxRep("didnt_skip2", res, "(3 ((100) (200) 1) 22)");

        }
        TestScore();
    }
    catch(const LispX &ex) {
        printf("Caught LispX: %s\n", ex.Description());
        doprint(ex.Parameter());
        //printstructure(ex.Parameter().GetPtr());
        printf("\n");
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    poc();
    LTheDefaultThread.ResetReturn();
    poc();
    return 0;
}


