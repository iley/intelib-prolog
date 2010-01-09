//   InteLib                                    http://www.intelib.org
//   The file tests/refal/t_rfexpr.cpp
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
#include "sexpress/sexpress.hpp"
#include "sexpress/sstring.hpp"

#include "refal/refal.hpp"



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
    poc();
#ifdef INTELIB_DEBUG_COUNTERS
    int before = SExpression::object_counter;
#endif
    try {
        TestSection("RefalExpressions");
        TestSubsection("TextRepresentation");
        {
            RfReference empty(new RfExpression);
            TESTTR("empty_rfexpr", empty, "");

            TESTTR("empty_subexpr", R|~R, "()");
        }
        TestSubsection("Construction");
        {
            TESTTR("single", (R|99), "99");
            TESTTR("plain", (R|1,2,3), "1 2 3");
            TESTTR("not_plain", (R|1,(R|2), (R|3,4,(R|5,6),7),8),
                   "1 (2) (3 4 (5 6) 7) 8");
            TESTTR("with_call", (R|1, R<2, 3, 4>R, 5), "1 <2 3 4> 5");
            TESTTR("just_a_call", (R<1,2,3,4>R), "<1 2 3 4>");
            TESTTR("just_a_call_1", (R<1>R), "<1>");
            TESTTR("just_a_call_2", (R<1,2>R), "<1 2>");
            TESTTR("call_in_call", (R<1,2, R<3,4,R<5,6,7>R,8>R,9>R),
                   "<1 2 <3 4 <5 6 7> 8> 9>");

            TESTTR("multy_within_expr", (R|1,2,R<1,R<2,R<3,4>R>R>R,5),
                   "1 2 <1 <2 <3 4>>> 5");
        }
        TestSubsection("MultyCallConstructor");
        {
          #if 0
            TESTTR("x", (R<R<1), "<#<#1");
            TESTTR("xx", (R<R<1, 2), "<#<#1 2");
            TESTTR("xxx", (R<R<1, 2>R), "<#<1 2>");
          #endif
            TESTTR("call_in_beginning_of_call", (R<R<1,2>R, 3>R),
                   "<<1 2> 3>");
            TESTTR("call_in_beginning_of_call_noclose", (R<R<1,2>R, 3),
                   "<#<1 2> 3");
            TESTTR("call_in_beginning_of_call2", (R<R<R<1,2>R, 3>R,4>R),
                   "<<<1 2> 3> 4>");
            TESTTR("call_in_beginning_of_call3", (R<R<R<1,2>R,3>R,4>R),
                   "<<<1 2> 3> 4>");


            TESTTR("multy_within_expr", (R|1,2,R<R<R<3,4>R>R>R,5),
                   "1 2 <<<3 4>>> 5");

            TESTTR("call_starts_with_subexpr",
                   (R|0,R<R<(R|1,2,3),4>R>R,5),
                   "0 <<(1 2 3) 4>> 5");

            TESTTR("inner_call_of_one_symbol",
                   (R<R<1>R>R), "<<1>>");

            TESTTR("inner_call_of_one_symbol",
                   (R|0,R<R<1>R>R,2), "0 <<1>> 2");

        }
        TestSubsection("Strings decomposition");
        {
            TESTTR("simple_char", (R|"a"), "\"a\"");
            TESTTR("char_shuffled", (R|1,"a",2), "1 \"a\" 2");
            TESTTR("char_shuffled2", (R|"a",2,"b"),
                "\"a\" 2 \"b\"");
            TESTTR("char_shuffled3", (R|"a",2,3,"b"), "\"a\" 2 3 \"b\"");
            TESTTR("char_shuffled4", (R|"a",2,3,"bc"),
                "\"a\" 2 3 \"bc\"");
            TESTTR("char_shuffled4", (R|"ab",2,3,"bc"),
                "\"ab\" 2 3 \"bc\"");
            TESTTR("char_shuffled4", (R|"ab",(R|2,3),"bc"),
                   "\"ab\" (2 3) \"bc\"");
            TESTTR("simple_strings", (R|"abcd","efgh","ijkl"),
                   "\"abcdefghijkl\"");
            TESTTR("strings_in_a_call", (R<"abcd","efgh","ijkl">R),
                   "<\"abcdefghijkl\">");
            TESTTR("one_strings_in_many_calls", (R<R<R<"abcd">R>R>R),
                   "<<<\"abcd\">>>");
        }
        TestSubsection("Pairing");
        {
            RfReference r((R|1,(R|2,(R|3,(R|4),5),6),7));
            RfListItem *temp1 = r->GetFirst(), *temp2 = r->GetLast();
            temp1 = temp1->next; temp2 = temp2->prev;
            for (int i=0; i<2; i++) {
                TESTB("paired_lr",
                     temp1->pair == 
                     temp2);
                TESTB("paired_rl",
                     temp2->pair == 
                     temp1);
                temp1 = temp1->next; temp2 = temp2->prev;
                temp1 = temp1->next; temp2 = temp2->prev;
            }

        }
        TestSubsection("ConvertToLisp");
        {
            RfReference r((R|1,(R|"abcd",(R|3,(R|4),"efgh"),6),7,"i"));
            SReference r1 = r->ConvertToLisp();
            TESTTR("convert_to_lisp", r1,
                   "(1 (\"abcd\" (3 (4) \"efgh\") 6) 7 \"i\")");

        }
        TestSubsection("Memory leaks");
        {
#ifdef INTELIB_DEBUG_COUNTERS
            TEST("no_leaks", SExpression::object_counter, before);
#endif
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


