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

#include "lisp/lisp.hpp"
#include "lisp/lsymbol.hpp"
#include "lisp/lcont.hpp"
#include "lisp/llambda.hpp"

#include "lfun_std.hpp"
#include "lfun_sel.hpp"


void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}


LListConstructor L;
LFunctionConstructor F;

LFunctionalSymbol<LFunctionCons> CONS("CONS");
LFunctionalSymbol<LFunctionSetq> SETQ("SETQ");
LFunctionalSymbol<LFunctionCar> CAR("CAR");
LFunctionalSymbol<LFunctionCdr> CDR("CDR");
LFunctionalSymbol<LFunctionCddar> CDDAR("CDDAR");
LFunctionalSymbol<LFunctionFirst> FIRST("FIRST");
LFunctionalSymbol<LFunctionThird> THIRD("THIRD");
LFunctionalSymbol<LFunctionList> LIST("LIST");
LFunctionalSymbol<LFunctionLength> LENGTH("LENGTH");
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
LFunctionalSymbol<LFunctionDifference> MINUS("MINUS");
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
LFunctionalSymbol<LFunctionMember> MEMBER("MEMBER");
LFunctionalSymbol<LFunctionMemberp> MEMBERP("MEMBERP");
LFunctionalSymbol<LFunctionMapcar> MAPCAR("MAPCAR");
LFunctionalSymbol<LFunctionLet> LET("LET");
LFunctionalSymbol<LFunctionFlet> FLET("FLET");
LFunctionalSymbol<LFunctionLabels> LABELS("LABELS");
LFunctionalSymbol<LFunctionApply> APPLY("APPLY");
LFunctionalSymbol<LFunctionFuncall> FUNCALL("FUNCALL");
LFunctionalSymbol<LFunctionSort> SORT("SORT");
LFunctionalSymbol<LFunctionRplaca> RPLACA("RPLACA");
LFunctionalSymbol<LFunctionRplacd> RPLACD("RPLACD");
LFunctionalSymbol<LFunctionWhile> WHILE("WHILE");
LFunctionalSymbol<LFunctionTextrepresentation>
TEXTREPRESENTATION("TEXTREPRESENTATION");
LFunctionalSymbol<LFunctionSetf> SETF("SETF");
LFunctionalSymbol<LFunctionAref> AREF("AREF");
LFunctionalSymbol<LFunctionMaparray> MAPARRAY("MAPARRAY");
LFunctionalSymbol<LFunctionNth> NTH("NTH");
LFunctionalSymbol<LFunctionRemove> REMOVE("REMOVE");
LFunctionalSymbol<LFunctionLreduce> LREDUCE("LREDUCE");
LFunctionalSymbol<LFunctionRreduce> RREDUCE("RREDUCE");
LFunctionalSymbol<LFunctionDefvar> DEFVAR("DEFVAR");

LReference LTrue(*PTheLispBooleanTrue);
LReference LFalse(*PTheLispBooleanFalse);

int main()
{
    poc();
    try {
        TestSection("Standard Functions");
        TestSubsection("CONS");
        {
            LReference res = (L|CONS, 1, 2).Evaluate();
            TESTTR("cons", res, "(1 . 2)");
        }
        TestSubsection("SETQ");
        {
            LSymbol A("A");
            LSymbol B("B");
            A->SetDynamicValue(22);
            B->SetDynamicValue(33);
            LReference res = (L|SETQ, A, ~B).Evaluate();
            TESTTR("setq_return", res, "B");
            TESTTR("setq_side_effect", LReference(A).Evaluate(), "B");
        }
        TestSubsection("Cars");
        {
            LReference
            res = (L|CAR, ~(L|1, 2, 3)).Evaluate();
            TESTTR("car", res, "1");
            res = (L|CDR, ~(L|1, 2, 3)).Evaluate();
            TESTTR("cdr", res, "(2 3)");
            res = (L|CDDAR, ~(L|(L|1, 2, 3), 4, 5)).Evaluate();
            TESTTR("cddar", res, "(3)");
            res = (L|CAR, ~(L)).Evaluate();
            TESTB("car_nil", res.IsEmptyList());
            res = (L|CDR, ~(L)).Evaluate();
            TESTB("cdr_nil", res.IsEmptyList());
            res = (L|CDDAR, ~(L)).Evaluate();
            TESTB("cddar_nil", res.IsEmptyList());
            res = (L|FIRST, ~(L|1, 2, 3, 4, 5)).Evaluate();
            TESTTR("first", res, "1");
            res = (L|THIRD, ~(L|1, 2, 3, 4, 5)).Evaluate();
            TESTTR("third", res, "3");
            res = (L|THIRD, ~(L|1, 2)).Evaluate();
            TESTB("third_nil", res.IsEmptyList());
        }
        TestSubsection("LIST");
        {
            LReference res;
            res = (L|LIST).Evaluate();
            TESTB("list_empty", res.IsEmptyList());
            res = (L|LIST, 1, 2, 3).Evaluate();
            TESTTR("list_1_2_3", res, "(1 2 3)");
            res = (L|LIST, (L|CAR, ~(L|1, 2, 3)), 4, 5).Evaluate();
            TESTTR("list_evals_params", res, "(1 4 5)");
        }
        TestSubsection("LENGTH");
        {
            LReference res;
            res = (L|LENGTH, ~(L)).Evaluate();
            TEST("length_empty", res.GetInt(), 0);
            res = (L|LENGTH, ~(L| 1, 2, 3)).Evaluate();
            TEST("length_regular", res.GetInt(), 3);
            res = (L|LENGTH, ~((L| 1, 2, 3)||4)).Evaluate();
            TEST("length_dotted", res.GetInt(), 3);
        }
        TestSubsection("DEFUN");
        {
            LSymbol A("A");
            LSymbol B("B");
            LSymbol F("F");
            LReference res;
            res = (L|DEFUN, F, (L|A, B), (L|LIST, A, B)).Evaluate();
            TESTTR("defun_returns_name", res, "F");
            TESTTR("defun_makes_lambda_object", F->GetFunction(),
                      "#<CLOSURE (A B)((LIST A B))>");
            res = (L|F, (L|CONS, 1, 2), ~(L|11, 22, 33)).Evaluate();
            TESTTR("defuned_function_works", res, "((1 . 2) (11 22 33))");
        }
        TestSubsection("DEFMACRO");
        {
            LSymbol A("A");
            LSymbol B("B");
            LSymbol F("F");
            LReference res;
            res = (L|DEFMACRO, F, (L|A, B), (L|LIST, A, B)).Evaluate();
            TESTTR("defmacro_returns_name", res, "F");
            TESTTR("defmacro_makes_macro_object", F->GetFunction(),
                      "#<MACRO (A B)((LIST A B))>");
            res = (L|F, LIST, 1).Evaluate();
            TESTTR("macro_works", res, "(1)");
        }
        TestSubsection("APPEND");
        {
            LReference res;
            res = (L|APPEND).Evaluate();
            TESTB("append_of_zero", res.IsEmptyList());
            res = (L|APPEND, ~(L|1, 2, 3), ~(L|4, 5, 6), ~(L|7, 8, 9)).Evaluate();
            TESTTR("append_of_lists", res, "(1 2 3 4 5 6 7 8 9)");
            res = (L|APPEND, ~(L|1, 2, 3), ~(L|4, 5, 6), 777).Evaluate();
            TESTTR("append_with_atomic_last", res, "(1 2 3 4 5 6 . 777)");
            LSymbol A("A");
            LSymbol B("B");
            A->SetDynamicValue((L|222,333));
            B->SetDynamicValue((L|555,666));
            res = (L|APPEND, A, B).Evaluate();
            TESTTR("append_evaluates_params", res, "(222 333 555 666)");
            res = LReference(A).Evaluate();
            TESTTR("append_is_nondestructive", res, "(222 333)");
            res = LReference(B).Evaluate();
            TESTTR("append_is_nondestructive2", res, "(555 666)");
            res = (L|APPEND, ~(L|1, 2, 3), ~(L|4, 5, 6), ~(L)).Evaluate();
            TESTTR("append_empty_at_the_end", res, "(1 2 3 4 5 6)");
            res = (L|APPEND, ~(L|1, 2, 3), ~(L), ~(L|4, 5, 6)).Evaluate();
            TESTTR("append_empty_in_the_middle", res, "(1 2 3 4 5 6)");
        }
        TestSubsection("NOT");
        {
            LReference res;
            res = (L|NOT, LReference(*PTheLispBooleanTrue)).Evaluate();
            TESTB("not_true", !res.IsTrue());
            res = (L|NOT, LReference(*PTheLispBooleanTrue)).Evaluate();
            TESTB("not_true_the_same_as_previous!!!", !res.IsTrue());
            res = (L|NOT, LReference(*PTheLispBooleanFalse)).Evaluate();
            TESTB("not_false", res.IsTrue());
            res = (L|NOT, 1).Evaluate();
            TESTB("not_arbitrary", !res.IsTrue());
            res = (L|NOT, ~L).Evaluate();
            TEST("not_empty_list", res.IsTrue(),
                 !LReference(*PTheEmptyList).IsTrue());
        }
        TestSubsection("NULL");
        {
            LReference res;


            res = (L|lNULL, LReference(*PTheLispBooleanFalse)).Evaluate();
            TEST("null_empty_list", res.IsTrue(),
                 PTheLispBooleanFalse->IsEmptyList());
            res = (L|lNULL, ~(L|1, 2)).Evaluate();
            TESTB("null_on_nonempty_list", !res.IsTrue());
            res = (L|lNULL, L).Evaluate();
            TESTB("null_on_empty_list", res.IsTrue());
            res = (L|lNULL, 1).Evaluate();
            TESTB("null_on_arbitrary_atom", !res.IsTrue());
        }
        TestSubsection("AND");
        {
            LReference res;
            //res = (L|AND).Evaluate();
            //TESTB("and_empty", res == LTrue);
            res = (L|AND, ~LTrue).Evaluate();
            TESTB("and_t", res == LTrue);
            res = (L|AND, ~LTrue, ~LTrue, ~LTrue).Evaluate();
            TESTB("and_t_t_t", res == LTrue);
            res = (L|AND, ~LFalse, ~LTrue, ~LTrue).Evaluate();
            TESTB("and_f_t_t", res == LFalse);
            res = (L|AND, ~LTrue, ~LFalse, ~LTrue).Evaluate();
            TESTB("and_t_f_t", res == LFalse);
            res = (L|AND, ~LTrue, ~LTrue, ~LFalse).Evaluate();
            TESTB("and_t_t_f", res == LFalse);
            LSymbol A("A");
            A->SetDynamicValue(22);
            res = (L|AND, ~LTrue, (L|SETQ, A, 36), 666).Evaluate();
            TESTTR("and_returns_last", res, "666");
            TESTTR("and_continues", LReference(A).Evaluate(), "36");
            res = (L|AND, ~LFalse, (L|SETQ, A, 49)).Evaluate();
            TESTTR("and_stops", LReference(A).Evaluate(), "36");
        }
        TestSubsection("OR");
        {
            LReference res;
            res = (L|OR).Evaluate();
            TESTB("or_empty", res == LFalse);
            res = (L|OR, ~LTrue).Evaluate();
            TESTB("or_t", res == LTrue);
            res = (L|OR, ~LFalse, ~LFalse, ~LFalse).Evaluate();
            TESTB("or_f_f_f", res == LFalse);
            res = (L|OR, ~LFalse, ~LFalse, ~LTrue).Evaluate();
            TESTB("or_f_f_t", res == LTrue);
            res = (L|OR, ~LTrue, ~LFalse, ~LFalse).Evaluate();
            TESTB("or_t_f_f", res == LTrue);
            res = (L|OR, ~LFalse, ~LTrue, ~LFalse).Evaluate();
            TESTB("or_f_t_f", res == LTrue);
            LSymbol A("A");
            A->SetDynamicValue(22);
            res = (L|OR, 666, 777, (L|SETQ, A, 36)).Evaluate();
            TESTTR("or_stops", LReference(A).Evaluate(), "22");
            TESTTR("or_returns_first", res, "666");
            res = (L|OR, ~LFalse, (L|SETQ, A, 49)).Evaluate();
            TESTTR("or_continues", LReference(A).Evaluate(), "49");
            TESTTR("or_return", res, "49");
        }
        TestSubsection("COND");
        {
            LReference res;
            res = (L|COND).Evaluate();
            TESTB("cond_empty", res == LFalse);
            LSymbol A("A");
            A->SetDynamicValue(1);
            res = (L|COND,
                   (L|LFalse),
                   (L|A, (L|SETQ, A, 55), 666),
                   (L|LTrue, (L|SETQ, A, 77))
                  ).Evaluate();
            TESTTR("cond_return", res, "666");
            TESTTR("cond_progn_and_stops", LReference(A).Evaluate(), "55");
        }
        TestSubsection("IF");
        {
            LReference res;
            res = (L|IF, ~LFalse, 1).Evaluate();
            TESTB("if_false_no_else", res == LFalse);
            LSymbol A("A");
            LSymbol B("B");
            A->SetDynamicValue(1);
            B->SetDynamicValue(1);
            res = (L|IF, ~LTrue, (L|SETQ, A, 2), (L|SETQ, B, 3)).Evaluate();
            TESTTR("if_true_res", res, "2");
            TESTTR("if_true_skips_false", LReference(B).Evaluate(), "1");
            A->SetDynamicValue(1);
            B->SetDynamicValue(1);
            res = (L|IF, ~LFalse, (L|SETQ, A, 2), (L|SETQ, B, 3)).Evaluate();
            TESTTR("if_false_res", res, "3");
            TESTTR("if_false_skips_true", LReference(A).Evaluate(), "1");
        }
        TestSubsection("PLUS");
        {
            LReference res;
            res = (L|PLUS).Evaluate();
            TESTTR("plus_empty", res, "0");

            res = (L|PLUS, "abcd").Evaluate();
            TESTTR("plus_string", res, "\"abcd\"");
            res = (L|PLUS, "abcd", "efgh", "ijkl").Evaluate();
            TESTTR("plus_several_string", res, "\"abcdefghijkl\"");

            res = (L|PLUS, 1).Evaluate();
            TESTTR("plus_int", res, "1");
            res = (L|PLUS, 1, 2, 3, 4).Evaluate();
            TESTTR("plus_several_integers", res, "10");


            res = (L|PLUS, 1.0).Evaluate();
            TESTTR("plus_float", res,
                      LReference(1.0)->TextRepresentation().c_str());
            res = (L|PLUS, 1.0, 2.0, 3.0, 4.25).Evaluate();
            TESTTR("plus_several_floats", res,
                      LReference(10.25)->TextRepresentation().c_str());

        }
        TestSubsection("DIFFERENCE");
        {
            LReference res;

            res = (L|DIFFERENCE, 1).Evaluate();
            TESTTR("difference_int", res, "-1");
            res = (L|DIFFERENCE, 10, 2, 3, 4).Evaluate();
            TESTTR("difference_several_ints", res, "1");

            res = (L|DIFFERENCE, 1.0).Evaluate();
            TESTTR("difference_float", res,
                      LReference(-1.0)->TextRepresentation().c_str());
            res = (L|DIFFERENCE, 11.0, 2.0, 3.0, 4.25).Evaluate();
            TESTTR("difference_several_floats", res,
                      LReference(1.75)->TextRepresentation().c_str());

        }
        TestSubsection("TIMES");
        {
            LReference res;
            res = (L|TIMES).Evaluate();
            TESTTR("times_empty", res, "1");

            res = (L|TIMES, 5).Evaluate();
            TESTTR("times_int", res, "5");
            res = (L|TIMES, 2, 3, 4).Evaluate();
            TESTTR("times_several_integers", res, "24");

            res = (L|TIMES, 2.0).Evaluate();
            TESTTR("times_float", res,
                      LReference(2.0)->TextRepresentation().c_str());
            res = (L|TIMES,  2.0, 3.0, 4.25).Evaluate();
            TESTTR("times_several_floats", res,
                      LReference(25.5)->TextRepresentation().c_str());

        }
        TestSubsection("QUOTIENT");
        {
            LReference res;

            res = (L|QUOTIENT, 2).Evaluate();
            TESTTR("quotient_int", res, "2");
            res = (L|QUOTIENT, 150, 2, 5, 3).Evaluate();
            TESTTR("quotient_several_integers", res, "5");

            res = (L|QUOTIENT, 2.0).Evaluate();
            TESTTR("quotient_float", res,
                      LReference(2.0)->TextRepresentation().c_str());
            res = (L|QUOTIENT, 150.150, 2.0, 5.0, 3.0).Evaluate();
            TESTTR("quotient_several_floats", res,
                      LReference(5.005)->TextRepresentation().c_str());

            res = (L|QUOTIENT, 21, 2).Evaluate();
            TESTTR("quotient_int_by_int_float_result", res,
                      LReference(10.5)->TextRepresentation().c_str());

        }
        TestSubsection("GREATERP");
        {
            LReference res;

            res = (L|GREATERP, 1, 2).Evaluate();
            TESTB("greaterp_int_f", res == LFalse);
            res = (L|GREATERP, 3, 2).Evaluate();
            TESTB("greaterp_int_t", res == LTrue);
            res = (L|GREATERP, 2, 2).Evaluate();
            TESTB("greaterp_int_not_equal", res == LFalse);

            res = (L|GREATERP, 1.0, 2.0).Evaluate();
            TESTB("greaterp_float_f", res == LFalse);
            res = (L|GREATERP, 3.25, 2).Evaluate();
            TESTB("greaterp_float_t", res == LTrue);
            res = (L|GREATERP, 2.77, 2.77).Evaluate();
            TESTB("greaterp_float_not_equal", res == LFalse);

            res = (L|GREATERP, 1, 1.01).Evaluate();
            TESTB("greaterp_mixed_f", res == LFalse);
            res = (L|GREATERP, 2.0001, 2).Evaluate();
            TESTB("greaterp_mixed_t", res == LTrue);
            res = (L|GREATERP, 0.99, 1).Evaluate();
            TESTB("greaterp_mixed_f2", res == LFalse);
            res = (L|GREATERP, 2, 1.99).Evaluate();
            TESTB("greaterp_mixed_t2", res == LTrue);
        }
        TestSubsection("LESSP");
        {
            LReference res;

            res = (L|LESSP, 1, 2).Evaluate();
            TESTB("lessp_int_f", res == LTrue);
            res = (L|LESSP, 3, 2).Evaluate();
            TESTB("lessp_int_t", res == LFalse);
            res = (L|LESSP, 2, 2).Evaluate();
            TESTB("lessp_int_not_equal", res == LFalse);

            res = (L|LESSP, 1.0, 2.0).Evaluate();
            TESTB("lessp_float_f", res == LTrue);
            res = (L|LESSP, 3.25, 2).Evaluate();
            TESTB("lessp_float_t", res == LFalse);
            res = (L|LESSP, 2.77, 2.77).Evaluate();
            TESTB("lessp_float_not_equal", res == LFalse);

            res = (L|LESSP, 1, 1.01).Evaluate();
            TESTB("lessp_mixed_f", res == LTrue);
            res = (L|LESSP, 2.0001, 2).Evaluate();
            TESTB("lessp_mixed_t", res == LFalse);
            res = (L|LESSP, 0.99, 1).Evaluate();
            TESTB("lessp_mixed_f2", res == LTrue);
            res = (L|LESSP, 2, 1.99).Evaluate();
            TESTB("lessp_mixed_t2", res == LFalse);
        }
        TestSubsection("MATHEQUAL");
        {
            LReference res;

            res = (L|MATHEQUAL, 1, 2).Evaluate();
            TESTB("mathequal_int", res == LFalse);
            res = (L|MATHEQUAL, 2, 1).Evaluate();
            TESTB("mathequal_int2", res == LFalse);
            res = (L|MATHEQUAL, 2, 2).Evaluate();
            TESTB("mathequal_int3", res == LTrue);
            res = (L|MATHEQUAL, 2.0, 2).Evaluate();
            TESTB("mathequal_mixed", res == LTrue);
            res = (L|MATHEQUAL, 2, 2.0).Evaluate();
            TESTB("mathequal_mixed2", res == LTrue);
        }
        TestSubsection("ATOM");
        {
            LReference res;
            LSymbol A("A");
            LSymbol B("B");
            A->SetDynamicValue(1);
            res = (L|ATOM, A).Evaluate();
            TESTB("atom_true", res.IsTrue());
            A->SetDynamicValue(L|A);
            B->SetDynamicValue(L|B);
            res = (L|ATOM, A).Evaluate();
            TESTB("atom_false", !res.IsTrue());
        }
        TestSubsection("NUMBERP");
        {
            LReference res;
            LSymbol A("A");
            A->SetDynamicValue(1);
            res = (L|NUMBERP, A).Evaluate();
            TESTB("numberp_int", res.IsTrue());
            A->SetDynamicValue(7.7);
            res = (L|NUMBERP, A).Evaluate();
            TESTB("numberp_float", res.IsTrue());
            A->SetDynamicValue("xyz");
            res = (L|NUMBERP, A).Evaluate();
            TESTB("numberp_false", !res.IsTrue());
        }
        TestSubsection("SYMBOLP");
        {
            LReference res;
            LSymbol A("A");
            A->SetDynamicValue(A);
            res = (L|SYMBOLP, A).Evaluate();
            TESTB("symbolp_true", res.IsTrue());
            A->SetDynamicValue(1);
            res = (L|SYMBOLP, A).Evaluate();
            TESTB("symbolp_false", !res.IsTrue());
        }
        TestSubsection("LISTP");
        {
            LReference res;
            LSymbol A("A");
            A->SetDynamicValue(1);
            res = (L|LISTP, A).Evaluate();
            TESTB("list_false", !res.IsTrue());
            A->SetDynamicValue(L|A);
            res = (L|LISTP, A).Evaluate();
            TESTB("list_true", res.IsTrue());
            A->SetDynamicValue(L|1||2);
            res = (L|LISTP, A).Evaluate();
            TESTB("list_dotpair_true", res.IsTrue());
        }
        TestSubsection("Equalities");
        {
            LReference res;
            LSymbol A("A");
            LSymbol B("B");
            res = (L|EQ, ~A, ~B).Evaluate();
            TESTB("eq_false", !res.IsTrue());
            res = (L|EQ, ~A, ~A).Evaluate();
            TESTB("eq_true", res.IsTrue());
            res = (L|EQ, 1, 1).Evaluate();
            TESTB("eq_equal_int_constants_false", !res.IsTrue());
            A->SetDynamicValue(1);
            B->SetDynamicValue(LReference(A).Evaluate());
            res = (L|EQ, A, B).Evaluate();
            TESTB("eq_same_int_constants_true", res.IsTrue());
            res = (L|EQL, ~A, ~B).Evaluate();
            TESTB("eql_false", !res.IsTrue());
            res = (L|EQL, 1, 2).Evaluate();
            TESTB("eql_false2", !res.IsTrue());
            res = (L|EQL, 1, 1).Evaluate();
            TESTB("eql_equal_int_constants", res.IsTrue());
            res = (L|EQL, 12.2, 12.2).Evaluate();
            TESTB("eql_equal_float_constants", res.IsTrue());
            res = (L|EQL, "abc", "abc").Evaluate();
            TESTB("eql_equal_string_constants", res.IsTrue());
            res = (L|EQL, ~(L|1, 2, "abc"), ~(L|1, 2, "abc")).Evaluate();
            TESTB("eql_fails_for_trees", !res.IsTrue());
            res = (L|EQUAL, ~(L|1, 2, "abc"), ~(L|1, 2, "abc")).Evaluate();
            TESTB("equal_works_for_trees", res.IsTrue());
        }
        TestSubsection("LAST");
        {
            LReference res;
            res = (L|LAST, ~(L)).Evaluate();
            TESTB("last_of_empty", res.IsEmptyList());
            res = (L|LAST, ~((L|1, 2, 3, 4)||5)).Evaluate();
            TESTTR("last_of_dotted", res, "(4 . 5)");
            res = (L|LAST, ~((L|1, 2, 3, 4)||5), 0).Evaluate();
            TESTTR("last_of_dotted_0", res, "5");
            res = (L|LAST, ~((L|1, 2, 3, 4)||5), 1).Evaluate();
            TESTTR("last_of_dotted_1", res, "(4 . 5)");
            res = (L|LAST, ~((L|1, 2, 3, 4)||5), 3).Evaluate();
            TESTTR("last_of_dotted_3", res, "(2 3 4 . 5)");
            res = (L|LAST, ~((L|1, 2, 3, 4)||5), 1700).Evaluate();
            TESTTR("last_of_dotted_too_many", res, "(1 2 3 4 . 5)");
        }
        TestSubsection("MEMBER");
        {
            LReference res;
            res = (L|MEMBER, "abc", ~(L|"aaa", "bbb", "ccc")).Evaluate();
            TESTB("member_fail", res == LFalse);
            res = (L|MEMBER, 1, ~(L|1, 2, 3)).Evaluate();
            TESTTR("member_ok", res, "(1 2 3)");
            res = (L|MEMBER, 1, ~(L|2, 3, 1)).Evaluate();
            TESTTR("member_ok2", res, "(1)");
            res = (L|MEMBER, 3, ~((L|1, 2, 3)||4)).Evaluate();
            TESTTR("member_ok_dotted", res, "(3 . 4)");
            res = (L|MEMBER, "ddd", ~((L|"aaa", "bbb", "abc")||"ddd")).Evaluate();
            TESTB("member_fail_dotted", res == LFalse);


            res = (L|MEMBER, 25, ~(L| 1,2,3,100,4),
                   ~(LESSP->GetFunction())).Evaluate();
            TESTTR("member_test_parameter", res, "(100 4)");
        }
        TestSubsection("MEMBERP");
        {
            LReference res;
            res = (L|MEMBERP, "abc", ~(L|"aaa", "bbb", "ccc")).Evaluate();
            TESTB("memberp_fail", res == LFalse);
            res = (L|MEMBERP, "abc", ~(L|"abc", "bbb", "ccc")).Evaluate();
            TESTB("memberp_ok", res == LTrue);
            res = (L|MEMBERP, "abc", ~(L|"aaa", "bbb", "abc")).Evaluate();
            TESTB("memberp_ok2", res == LTrue);
            res = (L|MEMBERP, "abc", ~((L|"aaa", "bbb", "abc")||"ddd")).Evaluate();
            TESTB("memberp_ok_dotted", res == LTrue);
            res = (L|MEMBERP, "ddd", ~((L|"aaa", "bbb", "abc")||"ddd")).Evaluate();
            TESTB("memberp_fail_dotted", res == LFalse);

            res = (L|MEMBERP, 25, ~(L| 1,2,3,100,4),
                   ~(LESSP->GetFunction())).Evaluate();
            TESTB("member_test_parameter", res == LTrue);
        }
        TestSubsection("MAPCAR");
        {
            LReference res;
            LSymbol A("A");
            LSymbol B("B");
            res = (L|MAPCAR, F^CONS,
                   ~(L| A, 25), ~(L|B, 36)).Evaluate();
            TESTTR("mapcar_on_function", res, "((A . B) (25 . 36))");
            res = (L|MAPCAR, F^CONS,
                   ~(L| A, 25, 36), ~(L|B, 36, 49)).Evaluate();
            TESTTR("mapcar_on_function2", res, "((A . B) (25 . 36) (36 . 49))");
            res = (L|MAPCAR, F^CAR,
                   ~(L|(L|A, 25), (L|B, 36))).Evaluate();
            TESTTR("mapcar_unary", res, "(A B)");
            res = (L|MAPCAR, F^CAR, ~(L)).Evaluate();
            TESTB("mapcar_empty", res.IsEmptyList());
        }
        TestSubsection("FunctionConstructor");
        {
            LReference res;
            LSymbol A("A");
            LSymbol B("B");
            LFunctionConstructor F;
            res = (L|MAPCAR, F^CONS, ~(L| A, 25), ~(L|B, 36)).Evaluate();
            TESTTR("function_constructor", res, "((A . B) (25 . 36))");
        }
        TestSubsection("LET");
        {
            LReference res;
            LSymbol A("A");
            LSymbol B("B");
            A->SetDynamicValue("dynA");
            B->SetDynamicValue("dynB");
            SString str;
            res = (L|LET, (L|A), A).Evaluate();
            TESTB("let", res.IsEmptyList());
            res = (L|LET, (L|A, B), (L|LIST, A, B)).Evaluate();
            str = (L|L, L)->TextRepresentation();
            TESTTR("let_2", res, str.c_str());
            res = (L|LET, (L|(L|A,(L|PLUS, 1, 2)), (L|B, "abc")),
                   (L|LIST, A, B)).Evaluate();
            TESTTR("let_vals", res, "(3 \"abc\")");
            res = (L|LET, (L), A).Evaluate();
            TESTTR("let_empty", res, "\"dynA\"");

            TESTTR("let_unbind_ok_a", LReference(A).Evaluate(), "\"dynA\"");
            TESTTR("let_unbind_ok_b", LReference(B).Evaluate(), "\"dynB\"");
        }
        TestSubsection("LET");
        {
            LReference res;
            LSymbol A("A");
            LSymbol B("B");
            LSymbol C("C");
            LSymbol D("D");
            LSymbol E("E");
            LSymbol F("F");

            res = (L| LET, (L| (L|A, 1), (L| B, 2), (L| C, 3), (L| D, 4),
                               (L|E, 5), (L| F, 6)),
                           (L|LIST, A, B, C, D, E, F)).Evaluate();
            TESTTR("let_six_vars", res, "(1 2 3 4 5 6)");
        }
        TestSubsection("FLET&LABELS");
        {
            LReference res;
            LSymbol F1("F1");
            LSymbol F2("F2");
            LSymbol A("A");
            LSymbol B("B");
            res = (L|FLET, (L| (L|F1, (L|A), (L|PLUS, A, 1)),
                            (L|F2, (L|A, B), (L|PLUS, A, B, 10))),
                   (L|F1, (L|F2, 200, 3000))
                  ).Evaluate();
            TESTTR("flet", res, "3211");

            (L| DEFUN, F1, (L|A), (L| TIMES, A, 10)).Evaluate();
            res = (L|LABELS, (L| (L|F2, (L|B), (L| PLUS, 7, (L| F1, B))),
                              (L|F1, (L|B), (L| TIMES, B, 1000))
                             ),
                   (L| F2, 3)).Evaluate();
            TESTTR("labels_scoping", res, "3007");
            res = (L|FLET, (L| (L|F2, (L|B), (L| PLUS, 7, (L| F1, B))),
                            (L|F1, (L|B), (L| TIMES, B, 1000))
                           ),
                   (L| F2, 3)).Evaluate();
            TESTTR("flet_scoping", res, "37");
        }
        TestSubsection("APPLY");
        {
            LReference res;

            res = (L|APPLY, F^PLUS, ~(L|1, 2)).Evaluate();
            TESTTR("apply_plus", res, "3");

            res = (L|APPLY, F^PLUS, 10, 100, ~(L|1, 2)).Evaluate();
            TESTTR("apply_plus2", res, "113");

            res = (L|APPLY, F^PLUS, (L)).Evaluate();
            TESTTR("apply_noargs", res, "0");

            res = (L|APPLY, ~LReference(new LFunctionPlus), 1, 2, ~(L|3)
                  ).Evaluate();
            TESTTR("apply_on_functional_object", res, "6");

            res = (L|APPLY, F^CONS, ~(L|(L|PLUS, 2, 3), 4)).Evaluate();
            TESTTR("apply_last_element_quoted", res, "((PLUS 2 3) . 4)");
        }
        TestSubsection("FUNCALL");
        {
            LReference res;

            res = (L|FUNCALL, F^PLUS, 1, 2).Evaluate();
            TESTTR("funcall_plus", res, "3");

            res = (L|FUNCALL, F^PLUS, 10, 100, 1, 2).Evaluate();
            TESTTR("funcall_plus2", res, "113");

            res = (L|FUNCALL, F^PLUS).Evaluate();
            TESTTR("funcall_noargs", res, "0");

            res = (L|FUNCALL, ~LReference(new LFunctionPlus), 1, 2, 3).Evaluate();
            TESTTR("funcall_on_functional_object", res, "6");
        }
        TestSubsection("SORT");
        {
            LReference res;

            res = (L|SORT, ~(L|1, 2, 3, 55, 4, 5, 94), F^LESSP).Evaluate();
            TESTTR("sort", res, "(1 2 3 4 5 55 94)");

            res = (L|SORT, ~(L|1, 2, 3, 55, 4, 5, 94), F^GREATERP).Evaluate();
            TESTTR("sort_reversed", res, "(94 55 5 4 3 2 1)");

            res = (L|SORT, ~(L|1, 2, 3, 55, 4, 5, 94), F^GREATERP,
                   F^DIFFERENCE).Evaluate();
            TESTTR("sort_with_selector", res, "(1 2 3 4 5 55 94)");

            res = (L|SORT, (L), F^GREATERP, F^DIFFERENCE).Evaluate();
            TESTB("sort_nothing", res.IsEmptyList());
        }
        TestSubsection("RPLACA");
        {
            LReference res;
            LSymbol A("A");

            A->SetDynamicValue((L|1, 2, 3, 4));
            res = (L|RPLACA, A, 111).Evaluate();

            TESTTR("rplaca", LReference(A).Evaluate(), "(111 2 3 4)");
            TESTTR("rplaca_return_value", res, "(111 2 3 4)");
        }
        TestSubsection("RPLACD");
        {
            LReference res;
            LSymbol A("A");

            A->SetDynamicValue((L|1, 2, 3, 4));
            res = (L|RPLACD, A, 222).Evaluate();

            TESTTR("rplacd", LReference(A).Evaluate(), "(1 . 222)");
            TESTTR("rplacd_return_value", res, "(1 . 222)");
        }
        TestSubsection("WHILE");
        {
            LReference res;
            LSymbol A("A");
            LSymbol B("B");
            LSymbol C("C");

            A->SetDynamicValue(0);
            C->SetDynamicValue(0);
            res = (L|WHILE, (L|LESSP, A, 30),
                      (L|SETQ, B, (L|PLUS, A, 3)),
                      (L|SETQ, A, B),
                      (L|SETQ, C, (L|PLUS, C, 1))).Evaluate();

            TESTTR("while", LReference(A).Evaluate(), "30");
            TESTTR("while_numiterations", LReference(C).Evaluate(), "10");
        }
        TestSubsection("TEXTREPRESENTATION");
        {
            LReference r = (L|"a","b",25,(L|36));
            TEST("textrepresentation",
                 (L|TEXTREPRESENTATION, ~r).Evaluate().GetString(),
                 r.TextRepresentation().c_str());
        }
        TestSubsection("AREF");
        {
            SVector vec;
            vec[5] = 5;
            vec[6] = 6;
            TESTTR("aref", (L|AREF, ~LReference(vec), 5).Evaluate(), "5");
            (L|SETF, (L|AREF, ~LReference(vec), 6), 36).Evaluate();
            TESTTR("aref_ref", (L|AREF, ~LReference(vec), 6).Evaluate(), "36");
            (L|SETF, (L|AREF, ~LReference(vec), 7), 49).Evaluate();
            TESTTR("aref_setnew", (L|AREF, ~LReference(vec), 7).Evaluate(), "49");
        }
        TestSubsection("MAPARRAY");
        {
            SVector vec;
            LSymbol X("X");
            LSymbol Y("Y");
            for(int i=0; i<=9; i++) vec[i] = LReference(i*100 + i);
            Y->SetDynamicValue(0);
            LReference res =
                (L|MAPARRAY,
                 F^(L|LAMBDA, (L|X), (L|SETQ, Y, (L|PLUS, Y, X))),
                 ~vec
                ).Evaluate();
            TESTTR("maparray_return", res, "10");
            TESTTR("maparray_doing", Y->GetDynamicValue(), "4545");
        }
        TestSubsection("NTH");
        {
            TESTTR("nth", (L|NTH, 3, ~(L|11,22,33,44,55)).Evaluate(), "44");
            LReference ref((L|1,2,3,4,5));
            (L|SETF, (L|NTH, 2, ~ref), 333).Evaluate();
            TESTTR("nth_ref", ref, "(1 2 333 4 5)");
        }
        TestSubsection("REMOVE");
        {
            TESTTR("remove", (L|REMOVE, 3, ~(L|1,2,3,4,5,6,7,3)).Evaluate(),
                      "(1 2 4 5 6 7)");
            TESTTR("remove_eq",
                      (L|REMOVE, "abc",
                       ~(L|"abc", "xyz", "a","abc"),
                       F^EQ
                      ).Evaluate(),
                      "(\"abc\" \"xyz\" \"a\" \"abc\")");

            LReference ls((L|1,2,3,4));
            TESTB("remove_nop_keeps_eq", (L|REMOVE, 5, ~ls).Evaluate().GetPtr()
                 == ls.GetPtr());

        }
        TestSubsection("LREDUCE");
        {
            TESTTR("left_reduce_cons",
                      (L|LREDUCE, F^CONS, ~(L|1,2,3)).Evaluate(),
                      "((1 . 2) . 3)");
            TESTTR("left_reduce_cons_init",
                      (L|LREDUCE, F^CONS, ~(L|1,2,3), 100).Evaluate(),
                      "(((100 . 1) . 2) . 3)");
            TESTTR("left_reduce_one",
                      (L|LREDUCE, F^CONS, ~(L|1)).Evaluate(),
                      "1");
            TESTTR("left_reduce_init_only",
                      (L|LREDUCE, F^CONS, ~(L), 100).Evaluate(),
                      "100");
            TESTTR("left_reduce_minus",
                      (L|LREDUCE, F^MINUS, ~(L|1,2,3)).Evaluate(),
                      "-4");
            TESTTR("left_reduce_init",
                      (L|LREDUCE, F^MINUS, ~(L|1,2,3), 10).Evaluate(),
                      "4");
        }
        TestSubsection("RREDUCE");
        {
            TESTTR("right_reduce_cons",
                      (L|RREDUCE, F^CONS, ~(L|1,2,3)).Evaluate(),
                      "(1 2 . 3)");
            TESTTR("right_reduce_cons_init",
                      (L|RREDUCE, F^CONS, ~(L|1,2,3), 100).Evaluate(),
                      "(1 2 3 . 100)");
            TESTTR("right_reduce_one",
                      (L|RREDUCE, F^CONS, ~(L|1)).Evaluate(),
                      "1");
            TESTTR("right_reduce_init_only",
                      (L|RREDUCE, F^CONS, ~(L), 100).Evaluate(),
                      "100");
            TESTTR("right_reduce_minus",
                      (L|RREDUCE, F^MINUS, ~(L|1,2,3)).Evaluate(),
                      "2");
            TESTTR("right_reduce_init",
                      (L|RREDUCE, F^MINUS, ~(L|1,2,3), 10).Evaluate(),
                      "-8");
        }
        TestSubsection("LexicalBinding");
        {
            LSymbol A("A");
            LSymbol B("B");
            LSymbol F("F");
            LispContinuation cont;
            (L|SETQ, A, "glob_A").Evaluate(cont);
            (L|SETQ, B, "glob_B").Evaluate(cont);
            (L|DEFUN, F, (L| B), (L|LIST, A, B)).Evaluate(cont);
            SReference res =
                (L|LET, (L| (L| A, "lex_A"), (L|B, "lex_B")),
                    (L|LIST, A, B, (L|F, "param"))
                ).Evaluate(cont);
            TESTTR("lexic_binding_in_let_and_func", res,
                    "(\"lex_A\" \"lex_B\" (\"glob_A\" \"param\"))");
        }
        TestSubsection("DifferentBindingsInLet");
        {
            LSymbol A("A");
            LSymbol B("B");
            LSymbol F("F");
            LSymbol X("X");
            LispContinuation cont;
            (L|SETQ, A, "global").Evaluate(cont);
            (L|DEFVAR, B, "dynamic").Evaluate(cont);
            (L|DEFUN, F, (L| X), (L|LIST, X, A, B)).Evaluate(cont);
            SReference res =
                (L|LET, (L| (L| A, "lex"), (L|B, "local")),
                    (L|LIST, A, B, (L|F, "param"))
                ).Evaluate(cont);
            TESTTR("lexic_and_dynamic_binding", res,
                    "(\"lex\" \"local\" (\"param\" \"global\" \"local\"))");
            TESTTR("dyn_bind_restored", LReference(B).Evaluate(cont),
                    "\"dynamic\"");
        }
        TestSubsection("DifferentBindingsInFunction");
        {
            LSymbol A("A");
            LSymbol B("B");
            LSymbol F("F");
            LSymbol G("G");
            LSymbol X("X");
            LispContinuation cont;
            (L|SETQ, A, "global").Evaluate(cont);
            (L|DEFVAR, B, "dynamic").Evaluate(cont);
            (L|DEFUN, F, (L| X), (L|LIST, X, A, B)).Evaluate(cont);
            (L|DEFUN, G, (L|A,B), (L|LIST, A, B, (L|F, "param")))
                                                   .Evaluate(cont);
            SReference res = (L|G, "lexic", "local").Evaluate(cont); 
            TESTTR("lexic_and_dynamic_binding_in_function", res,
                    "(\"lexic\" \"local\" (\"param\" \"global\" \"local\"))");
            TESTTR("dyn_bind_restored_fun", LReference(B).Evaluate(cont),
                    "\"dynamic\"");
        }
        TestSubsection("LexicVarAccess");
        {
            LSymbol X("X");
            LispContinuation cont;
            LContextRef lex(new LExpressionContext);
            X->SetDynamicValue("dynamic");
            *(lex->ProvideBinding(X.GetPtr())) = "lexic";

            cont.PushTodo(IntelibContinuation::just_evaluate, X);
            cont.PushTodo(IntelibContinuation::set_context, lex);
            while(!cont.Ready()) cont.Step();
            SReference res = cont.Get();
            TESTTR("lexic_var_access", res, "\"lexic\"");
        }
        TestSubsection("DynamicVarOptimization");
        {
            LSymbol X("X");
            LispContinuation cont;
            LContextRef lex(new LExpressionContext);
            X->SetDynamicValue("dynamic");
            *(lex->ProvideBinding(X.GetPtr())) = "lexic";
            X->SetDynamicBinding();

            cont.PushTodo(IntelibContinuation::just_evaluate, X);
            cont.PushTodo(IntelibContinuation::set_context, lex);
            while(!cont.Ready()) cont.Step();
            SReference res = cont.Get();
            TESTTR("optimized_dynamic_var", res, "\"dynamic\"");
        }
        TestSubsection("LexicShadowing");
        {
            LSymbol A("A");
            LSymbol B("B");
            LSymbol C("C");
            LispContinuation cont;
            (L|SETQ, A, "global").Evaluate(cont);
	    (L|SETQ, B, "error_unchanged").Evaluate(cont);
            (L|SETQ, C, "error_unchanged").Evaluate(cont);
            SReference res =
                (L|LET, (L| (L| A, "lex")),
                    (L|LET, (L| (L| A, "inner")),
                         (L|SETQ, B, A)
                    ),
                    (L|SETQ, C, A)
                ).Evaluate(cont);
            TESTTR("lexic_shadow_inner", LReference(B).Evaluate(cont),
                                         "\"inner\"");
            TESTTR("lexic_shadowed",    LReference(C).Evaluate(cont),
                                         "\"lex\"");
            TESTTR("lexic_shadow_glob", LReference(A).Evaluate(cont),
                                         "\"global\"");
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


