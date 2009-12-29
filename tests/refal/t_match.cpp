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
#include "sexpress/sexpress.hpp"
#include "sexpress/sstring.hpp"

#include "refal/refal.hpp"
#include "refal/rvars.hpp"
#include "refal/rfmatch.hpp"



void doprint(const SExpression *ref)
{
    printf("%s\n", ref ? ref->TextRepresentation().c_str() : "(unbound)");
}

void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}


RfFormConstructor R;

int main()
{
    try {
        poc();
        TestSection("RefalMatches");
        TestSubsection("Substitute1");
        {
            RfVariable_S vs("vs");
            RfContext context;
            RfReference res;

            res = RefalSubstitute((R|1,2,3,4), context, false);
            TESTTR("substitute0", res, "1 2 3 4");

            RfReference expr = (R|11,22,33,44,55);

            RfListItem *temp1 = expr->GetFirst(), *temp2 = expr->GetLast();
            temp1 = temp1->next;
            temp1 = temp1->next;
            temp2 = temp2->prev;
            temp2 = temp2->prev;

            context.AddVariable(vs.GetPtr(), temp1, temp2, 0);

            res = RefalSubstitute((R|1,vs,2,vs,3,vs,4), context, false);
            TESTTR("substitute1", res, "1 33 2 33 3 33 4");

            res = RefalSubstitute((R|1,vs,(R|2,vs,(R|3,vs)),4), context,
                                  false);
            TESTTR("substitute1_tree", res, "1 33 (2 33 (3 33)) 4");

            res = RefalSubstitute((R|1,vs,R<2,vs,(R|3,vs)>R,4), context,
                                  false);
            TESTTR("substitute1_incall", res, "1 33 <2 33 (3 33)> 4");
        }
        TestSubsection("ConstMatches");
        {
            RfContext context;
            RfReference empty1(new RfExpression);
            RfReference empty2(new RfExpression);
            TESTB("empty_match",
                  RefalMatchProcess(empty1, empty2, context));
        }
        {
            RfContext context;
            TESTB("plain_const_match",
                  RefalMatchProcess((R|1,2,3), (R|1,2,3), context));
        }
        {
            RfContext context;
            TESTB("plain_const_unmatch",
                  !RefalMatchProcess((R|1,2,3), (R|1,3,1), context));
        }
        {
            RfContext context;
            TESTB("const_with_empty_list_match",
                  RefalMatchProcess((R|1,~R,3), (R|1,~R,3), context));
        }
        {
            RfContext context;
            TESTB("const_with_single_list_match",
                  RefalMatchProcess((R|1,(R|2,(R|3),4),5),
                                    (R|1,(R|2,(R|3),4),5), context));
        }
        {
            RfContext context;
            TESTB("const_with_single_list_match",
                  RefalMatchProcess((R|1,(R|2),3),(R|1,(R|2),3), context));
        }
        {
            RfContext context;
            TESTB("tree_const_match",
                  RefalMatchProcess((R|1,2,(R|3,4,(R|5,6),7,8),9,10),
                                    (R|1,2,(R|3,4,(R|5,6),7,8),9,10),
                                    context));
        }
        {
            RfContext context;
            TESTB("tree_const_unmatch",
                  !RefalMatchProcess((R|1,(R|2,3,(R|4)),5),
                                     (R|1,(R|2,7,(R|4)),5),
                                     context));
        }
        TestSubsection("VariableMatches");
        {
            RfContext context;
            RfVariable_S vs("vs");
            TESTB("s_var_match1",
                  RefalMatchProcess((R|2), (R|vs), context));
            RfReference res = RefalSubstitute((R|vs), context, false);
            TESTTR("s_var_match1_val", res, "2");
        }
        {
            RfContext context;
            RfVariable_S vs("vs");
            TESTB("s_var_match",
                  RefalMatchProcess((R|1,2,3), (R|1,vs,3), context));
            RfReference res = RefalSubstitute((R|vs), context, false);
            TESTTR("s_var_match_val", res, "2");
        }
        {
            RfContext context;
            RfVariable_S vs("vs");
            TESTB("s_var_doesnt_match_expr",
                  !RefalMatchProcess((R|1,(R|2),3), (R|1,vs,3), context));
        }
        {
            RfContext context;
            RfVariable_S vs("vs");
            TESTB("s_var_matches_the_same",
                  RefalMatchProcess((R|1,2,1), (R|vs,2,vs), context));
        }
        {
            RfContext context;
            RfVariable_S vs("vs");
            TESTB("s_var_doesnt_match_different",
                  !RefalMatchProcess((R|1,2,3), (R|vs,2,vs), context));
        }
        {
            RfContext context;
            RfVariable_T vt("vt");
            TESTB("t_var_does_match_expr",
                  RefalMatchProcess((R|1,(R|2),3), (R|1,vt,3), context));
            RfReference res = RefalSubstitute((R|vt), context, false);
            TESTTR("t_var_match_val", res, "(2)");
        }
        {
            RfContext context;
            RfVariable_E ve("ve");
            TESTB("e_var_closed_match",
                  RefalMatchProcess((R|1,2,3,4,5), (R|1,ve,5), context));
            RfReference res = RefalSubstitute((R|ve), context, false);
            TESTTR("e_var_closed_match_val", res, "2 3 4");
        }
        {
            RfContext context;
            RfVariable_S s1("s1");
            RfVariable_E e2("e2");
            TESTB("palindrom_match",
                  RefalMatchProcess((R|1,2,2,1),
                                    (R|s1,e2,s1), context));
            RfReference res = RefalSubstitute((R|s1, "/", e2), context,
                                              false);
            TESTTR("palindrom_match_val", res, "1 \"/\" 2 2");
        }
        {
            RfContext context;
            RfVariable_S s1("s1");
            RfVariable_E e2("e2");
            TESTB("palindrom_match",
                  RefalMatchProcess((R|"abcdefedcba"),
                                    (R|s1,e2,s1), context));
            RfReference res = RefalSubstitute((R|s1, "/", e2), context,
                                              false);
            TESTTR("palindrom_match_val", res, "\"a/bcdefedcb\"");
        }
        {
            RfContext context;
            RfVariable_S s1("s1");
            TESTB("try_match_empty_against_bound",
                  !RefalMatchProcess((R|1,2,3,4),
                                     (R|s1,2,3,4,s1), context));
        }
        {
            RfContext context;
            RfVariable_S s1("s1");
            TESTB("match_empty_against_s",
                  !RefalMatchProcess(~R, (R|s1), context));
        }
        {
            RfContext context;
            RfVariable_E e1("e1");
            TESTB("match_empty_against_e",
                  RefalMatchProcess(~R, (R|e1), context));
            RfReference res = RefalSubstitute((R|e1), context, false);
            TESTTR("match_empty_against_e_val", res, "");
        }
        {
            RfContext context;
            RfVariable_E ve1("ve1");
            TESTB("e_var_two_same_vars1",
                  RefalMatchProcess((~R), (R|ve1,ve1),
                                    context));
            RfReference res = RefalSubstitute((R|ve1), context, false);
            TESTTR("e_var_two_same_vars_value1", res, "");
        }
        {
            RfContext context;
            RfVariable_E ve1("ve1");
            TESTB("e_var_two_same_vars2",
                  !RefalMatchProcess((R|1,2,3), (R|ve1,ve1),
                                    context));
        }
        {
            RfContext context;
            RfVariable_E ve1("ve1");
            RfVariable_E ve2("ve2");
            TESTB("e_var_two_same_vars3",
                  RefalMatchProcess((R|1,2,3,4), (R|ve1,ve2,ve1),
                                    context));
            RfReference res = RefalSubstitute((R|ve2,ve1,ve2), context,
                                              false);
            TESTTR("e_var_two_same_vars_value3", res, "1 2 3 4 1 2 3 4");
        }
        TestSubsection("BackTracking");
        {
            RfContext context;
            RfVariable_E ve1("ve1");
            RfVariable_E ve2("ve2");
            TESTB("e_var_multiple",
                  RefalMatchProcess((R|1,2,3,4,5), (R|1,ve1,ve2,5),
                                    context));
            RfReference res = RefalSubstitute((R|ve1,99,ve2), context,
                                              false);
            TESTTR("e_var_multiple_last_val", res, "99 2 3 4");
        }
        TestSubsection("Crash0");
        {
            RfContext context;
            RfVariable_E e_1("e_1");
            RfVariable_E e_2("e_2");
            RfVariable_E e_3("e_3");
            RfVariable_E e_4("e_4");

            SLabel Extern_("Extern_");
            SLabel Var_("Var_");
            SLabel Id_("Id_");

            RfReference r1((R|~R, Extern_, (R|Id_, "xxx")));
            RfReference r2((R|
                            (R|e_1),
                            e_2,
                            (R|Var_, e_3),
                            e_4
                           ));
            TESTB("no_crash", !RefalMatchProcess(r1, r2, context));
        }
        TestSubsection("Crash1");
        {
            RfContext context;
            RfVariable_E e_1("e_1");
            RfVariable_S s_2("s_2");
            RfVariable_E e_3("e_3");

            RfReference r1((R| "abcdefghij"));
            RfReference r2((R|e_1, s_2, e_3));


            RfReference r11((R| "1234d56"));
            RfListItem *temp1 = r11->GetFirst();
            RfListItem *temp2 = r11->GetLast();
            for (int i=0; i<4; i++) temp1 = temp1->next;
            for (int i=0; i<2; i++) temp2 = temp2->prev;
            context.AddVariable(s_2.GetPtr(), temp1, temp2, 0);

            TESTB("no_crash1", RefalMatchProcess(r1, r2, context));
            RfReference res = RefalSubstitute((R|e_1, "/", s_2, "/", e_3),
                                              context,
                                              false);
            TESTTR("no_crash_value1", res, "\"abc/d/efghij\"");
        }
        TestSubsection("EVarsOverMarkup");
        {
            RfContext context;
            RfVariable_E e_1("e_1");
            RfVariable_E e_2("e_2");
            RfVariable_E e_3("e_3");

            RfReference r1((R| "a", (R|"b"), "c"));
            RfReference r2((R|e_1, (R|e_2), e_3));


            TESTB("e_markup_1", RefalMatchProcess(r1, r2, context));
            RfReference res = RefalSubstitute((R|e_1, "/", e_2, "/", e_3),
                                              context,
                                              false);
            TESTTR("e_markup_1_value", res, "\"a/b/c\"");

        }
        {
            RfContext context;
            RfVariable_E e_1("e_1");
            RfVariable_E e_2("e_2");
            RfVariable_E e_3("e_3");

            RfReference r1((R| "a", (R|"bcd"), "e"));
            RfReference r2((R|e_1, (R|"b", e_2), e_3));

            TESTB("e_markup_2", RefalMatchProcess(r1, r2, context));
            RfReference res = RefalSubstitute((R|e_1, "/", e_2, "/", e_3),
                                              context,
                                              false);
            TESTTR("e_markup_2_value", res, "\"a/cd/e\"");

        }
        {
            RfContext context;
            RfVariable_E e_1("e_1");
            RfVariable_E e_2("e_2");
            RfVariable_E e_3("e_3");

            RfReference r1((R| "a", (R|"bcd"), "e"));
            RfReference r2((R|(R|"b", e_2), e_3));

            TESTB("e_markup_3", !RefalMatchProcess(r1, r2, context));
        }
        {
            RfContext context;
            RfVariable_E e_1("e_1");
            RfVariable_E e_2("e_2");

            RfReference r1((R| "a", (R|"bcd"), "1", (R|"234")));
            RfReference r2((R|e_1, "1", e_2));

            TESTB("e_markup_4", RefalMatchProcess(r1, r2, context));

            RfReference res = RefalSubstitute((R|e_2), context, false);
            TESTTR("e_markup_4_value", res, "(\"234\")");

        }
        {
            RfContext context;
            RfVariable_E e_1("e_1");
            RfVariable_E e_2("e_2");

            RfReference r1((R|(R|1, 2), 1));
            RfReference r2((R|(R|e_1, e_2), e_1));

            TESTB("e_markup_5", RefalMatchProcess(r1, r2, context));

            RfReference res = RefalSubstitute((R|(R|e_1), (R|e_2)),
                                              context,
                                              false);
            TESTTR("e_markup_5_value", res, "(1) (2)");

        }
        TestScore();
    }
    catch(const IntelibX &ex)
    {
        printf("Caught IntelibX: %s\n", ex.Description());
        doprint(ex.Parameter().GetPtr());
        printf("\n");
    }
    catch(...)
    {
        printf("Something strange caught\n");
    }
    poc();
    return 0;
}


