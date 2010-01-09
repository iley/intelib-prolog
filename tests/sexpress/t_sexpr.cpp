//   InteLib                                    http://www.intelib.org
//   The file tests/sexpress/t_sexpr.cpp
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
#include <string.h>
#include "tests.hpp"

#include "sexpress/sexpress.hpp"
#include "sexpress/sstring.hpp"
#include "sexpress/iexcept.hpp"
#include "sexpress/gensref.hpp"

int constructed = 0;
int destructed = 0;

class MyTestTerm : public SExpression {
    int i;
public:
    static IntelibTypeId TypeId;
    MyTestTerm(int a) : SExpression(TypeId), i(a) { constructed ++; }
    ~MyTestTerm() { destructed ++; }
    int Get() const { return i; }
    SString TextRepresentation() const { return SString(""); }
};

class MyTestTerm2 : public SExpression {
    int i;
public:
    static IntelibTypeId TypeId;
    MyTestTerm2(int a) : SExpression(TypeId), i(a) { constructed ++; }
    ~MyTestTerm2() { destructed ++; }
    int Get() const { return i; }
    SString TextRepresentation() const { return SString(""); }
};

class MyTestTerm22 : public SExpression {
    int i;
public:
    static IntelibTypeId TypeId;
    MyTestTerm22(int a) : SExpression(TypeId), i(a) { constructed ++; }
    ~MyTestTerm22() { destructed ++; }
    int Get() const { return i; }
    SString TextRepresentation() const { return SString(""); }
};

IntelibTypeId MyTestTerm::TypeId(&SExpression::TypeId);
IntelibTypeId MyTestTerm2::TypeId(&SExpression::TypeId);
IntelibTypeId MyTestTerm22::TypeId(&MyTestTerm2::TypeId);

int main()
{
    try {
        TestSection("StdTerms");
        TestSubsection("TermGarbage");
        {
            MyTestTerm *term1 = new MyTestTerm(10);
            MyTestTerm2 *term2 = new MyTestTerm2(20);
            SReference ref1(term1);
            SReference ref2(term2);
            SReference ref3(new MyTestTerm(30));
            SReference ref4(new MyTestTerm2(40));
            TEST("constructed", constructed, 4);
            TEST("noone-destructed", destructed, 0);
            ref4 = new MyTestTerm(300);
            TEST("assign_ptr", destructed, 1);
            ref4 = ref1;
            TEST("assign_ref", destructed, 2);
            ref3 = 0;
            TEST("assign_null", destructed, 3);
        }
        TEST("correctness", constructed, destructed);
        {
            constructed = destructed = 0;
            SReference ref1(new MyTestTerm(200));
            SReference ref2(new MyTestTerm(300));
            TESTB("before_assigning_refs",constructed == 2 && destructed == 0);
            ref1 = ref2;
            TEST("after_assigning_refs", destructed, 1);
        }
        TestSubsection("TermType");
        {
            SReference ref5(new MyTestTerm(50));
            TESTB("type-of", ref5->TermType() == MyTestTerm::TypeId);
            TESTB("not-type-of", ref5->TermType() != MyTestTerm2::TypeId);
            TESTB("subtype", ref5->TermType().IsSubtypeOf(SExpression::TypeId));
            TESTB("not-subtype",
                 !(ref5->TermType().IsSubtypeOf(MyTestTerm2::TypeId)));
            SReference ref6(new MyTestTerm22(60));
            TESTB("subtype2_", ref6->TermType() == MyTestTerm22::TypeId);
            TESTB("22subE",
                MyTestTerm22::TypeId.IsSubtypeOf(SExpression::TypeId));
            TESTB("22sub2",
                MyTestTerm22::TypeId.IsSubtypeOf(MyTestTerm2::TypeId));
            TESTB("2subE",
                MyTestTerm2::TypeId.IsSubtypeOf(SExpression::TypeId));
            TESTB("subtype2",
                ref6->TermType().IsSubtypeOf(SExpression::TypeId));
            TESTB("subtype22",
                ref6->TermType().IsSubtypeOf(MyTestTerm2::TypeId));
            TESTB("subtype22_self",
                 ref6->TermType().IsSubtypeOf(MyTestTerm22::TypeId));
            SReference ref7(new MyTestTerm2(70));
            TESTB("not-subtype22",
                 !(ref7->TermType().IsSubtypeOf(MyTestTerm22::TypeId)));
        }
        TestSubsection("TermCasts");
        {
            SReference ref(new MyTestTerm(50));
            SReference ref2(new MyTestTerm2(70));
            TESTB("cast_to_lterm", ref.DynamicCastGetPtr<SExpression>()
                 == ref.GetPtr());
            TESTB("cast_to_lterm2", ref.DynamicCastGetPtr<SExpression>()
                 == ref.GetPtr());
            TESTB("cast_to_itself", ref.DynamicCastGetPtr<MyTestTerm>() ==
                 ref.GetPtr());
            TESTB("failed_cast", ref.DynamicCastGetPtr<MyTestTerm2>()==0);
        }
        TestSubsection("SExpressionCasts");
        {
            SReference ref(new MyTestTerm(50));
            SReference ref2(new MyTestTerm2(70));
            TESTB("cast_to_lterm", ref.DynamicCastGetPtr<SExpression>()
                 == ref.GetPtr());
            TESTB("cast_to_lterm2", ref.DynamicCastGetPtr<SExpression>()
                 == ref.GetPtr());
            TESTB("cast_to_itself", ref.DynamicCastGetPtr<MyTestTerm>()
                 == ref.GetPtr());
            TESTB("failed_cast", ref.DynamicCastGetPtr<MyTestTerm2>()==0);
        }
        TestSubsection("BasicTerms");
        {
            SReference intref(25);
            TESTC("integer_term", intref->TermType(), SExpressionInt::TypeId);
            TEST("integer_value", intref.GetInt(), 25);
            SReference floatref(25.0);
            TESTC("float_term", floatref->TermType(), SExpressionFloat::TypeId);
            TESTC("float_value", floatref.GetFloat(), (intelib_float_t)25.0);
            SReference strref("A_STRING");
            TESTC("pchar_term", strref->TermType(), SExpressionString::TypeId);
            TESTC("pchar_value", strcmp((const char*)strref.GetString(),
                                        "A_STRING"),
                  0);
            SReference charref('a');
            TESTC("char_term", charref->TermType(), SExpressionChar::TypeId);
            TEST("char_value", charref.GetSingleChar(), 'a');

        }
        TestSubsection("SExpressionString");
        {
            SReference strref1(new SExpressionString("A", "B"));
            TEST("concatenation_1_1", strref1.GetString(), "AB");
            SReference strref2(new SExpressionString("AA", "BB"));
            TEST("concatenation_2_2", strref2.GetString(), "AABB");
            SReference strref3(new SExpressionString("AAA", "BBB"));
            TEST("concatenation_3_3", strref3.GetString(), "AAABBB");
            SReference strref4(new SExpressionString("AAAA", "BBBB"));
            TEST("concatenation_4_4", strref4.GetString(), "AAAABBBB");
            SReference strref5(new SExpressionString("AAAAA", "BBBBB"));
            TEST("concatenation_5_5", strref5.GetString(), "AAAAABBBBB");
            SReference strref6(new SExpressionString("A"));
            TEST("construction1", strref6.GetString(), "A");
            SReference strref7(new SExpressionString("AA"));
            TEST("construction2", strref7.GetString(), "AA");
            SReference strref8(new SExpressionString("AAA"));
            TEST("construction3", strref8.GetString(), "AAA");
            SReference strref9(new SExpressionString("AAAA"));
            TEST("construction4", strref9.GetString(), "AAAA");


        }
        TestSubsection("SString");
        {
            SString str1;
            TESTC("default_is_empty", str1.c_str()[0], '\0');
            SString str2("");
            TESTC("empty_strings", str1, str2);
            TESTC("empty_is_empty", str2.c_str()[0], '\0');
            SString str3("AAA");
            SString str4("BBB");
            SString str5 = str3 + str4;
            TEST("string_addition", str5.c_str(), "AAABBB");
            SString str6("CCC");
            str6 += "DDD";
            TEST("string_increment_by_pchar", str6.c_str(), "CCCDDD");
            SString str7("EEE");
            SString str8("FFF");
            str7 += str8;
            TEST("string_increment_by_string", str7.c_str(), "EEEFFF");

            SString str10("Final countdown");
            SString str11("Final");
            str11 += " countdown";
            TESTB("string_equals_itself", str10 == str10);
            TESTB("string_doesnt_differ_from_itself", !(str10 != str10));
            TESTB("two_equal_strings", str10 == str11);
            TESTB("equal_strings_dont_differ", !(str10 != str11));
            SString str12("Another string");
            TESTB("two_non_equal_strings", str11 != str12);
            TESTB("nonequals_non_equal", !(str11 == str12));
            SString str13;
            SString str14("");
            TESTB("empty_strings_equal", str13 == str14);
            TESTB("empty_strings_dont_differ", !(str13 != str14));
            SReference strref("my_string");
            SString str15(strref);
            TEST("string_from_lreference", str15.c_str(), "my_string");
            {
                int flag = 0;
                try {
                    SReference ref(25);
                    SString str(ref);
                }
                catch(IntelibX_not_a_string lx)
                {
                    flag = 0x56;
                }
                TESTB("string_from_int_fails", flag == 0x56);
            }
        }
        TestSubsection("TextRepresentation");
        {
            SReference intref(100);
            TEST("integer_text_rep", intref->TextRepresentation().c_str(),
                 "100");
            char buf[100];
            intelib_float_t fl;
            fl = 100.1;
            SReference fltref(fl);
            snprintf(buf, sizeof(buf), INTELIB_FLOAT_FORMAT, fl);
            TEST("float_text_rep", fltref->TextRepresentation().c_str(),
                 buf);
            fl = 100.0;
            SReference fltref2(fl);
            snprintf(buf, sizeof(buf), INTELIB_FLOAT_FORMAT, fl);
            TEST("float2_text_rep", fltref2->TextRepresentation().c_str(),
                 buf);
            SReference strref("mystring");
            TEST("string_text_rep", strref->TextRepresentation().c_str(),
                 "\"mystring\"");
            SReference unbound;
            SReference unbound2;
            SReference unblist(unbound, unbound2);
            TEST("unbound_text_representation",
                 unblist->TextRepresentation().c_str(),
                 "(#<UNBOUND> . #<UNBOUND>)");
        }
        TestSubsection("DottedPairs");
        {
            SReference pairref(25, 36);
            TEST("pair_25_36", pairref->TextRepresentation().c_str(),
                 "(25 . 36)");
            SReference dotlistref(25, SReference(36, 49));
            TEST("dotlist_25_36_49", dotlistref->TextRepresentation().c_str(),
                 "(25 36 . 49)");
            SReference empty_list_ref(*PTheEmptyList);
            TEST("empty_list_ref", empty_list_ref->TextRepresentation().c_str(),
                 (*PTheEmptyList)->TextRepresentation().c_str());
            TESTB("empty_list_equal_empty", empty_list_ref.GetPtr() ==
                 PTheEmptyList->GetPtr());
            TESTB("dot_list_notequal_empty", dotlistref.GetPtr() !=
                 PTheEmptyList->GetPtr());

            SReference list25ref(25, *PTheEmptyList);
            TEST("list_1_elem", list25ref->TextRepresentation().c_str(),
                 "(25)");
            SReference list_16_25_ref(16, list25ref);
            TEST("cons_with_list", list_16_25_ref->TextRepresentation().c_str(),
                 "(16 25)");
            SReference list_of_lists_ref(list25ref,
                                         SReference(list25ref, *PTheEmptyList));
            TEST("list_of_lists", list_of_lists_ref->TextRepresentation().c_str(),
                 "((25) (25))");

        }
        TestSubsection("SExpressionLabel");
        {
            SExpressionLabel *lab1 = new SExpressionLabel("lab1");
            SReference labref(lab1);
            TEST("label", labref->TextRepresentation().c_str(),
                 "lab1");
            TESTB("label_equality", labref == SReference(lab1));
            SReference labref2(lab1);
            TESTB("label_equality2", labref== labref2);
            SReference ref3(25);
            TESTB("label_non_eq", labref != ref3);

#if 0 // no support for booleans in sexpression core
            TEST("boolean_true", LTheLispBooleanTrue.TextRepresentation().c_str(),
           #if CLSTYLE_BOOLEANS == 0
                 "#T"
           #else
                 "T"
           #endif
                );
#endif
        }
        TestSubsection("SListConstructor");
        {
            SListConstructor L;
            SReference list_int((L|25));
            TEST("list_of_1_int", list_int->TextRepresentation().c_str(),
                 "(25)");
            SReference list_str((L|"abcd"));
            TEST("list_of_1_str", list_str->TextRepresentation().c_str(),
                 "(\"abcd\")");
            intelib_float_t fl = 1.1;
            SReference list_float((L|fl));
            char buf[128];
            snprintf(buf, sizeof(buf), "(" INTELIB_FLOAT_FORMAT ")", fl);
            TEST("list_of_1_float", list_float->TextRepresentation().c_str(),
                 buf);
        }
        TestSubsection("ListConstructionAlgebra");
        {
            SListConstructor L;
            SReference listref((L|25, 36, 49, "abcd", "efgh"));
            TEST("plain_list", listref->TextRepresentation().c_str(),
                 "(25 36 49 \"abcd\" \"efgh\")");
            SReference listref2((L|(L|25), 36, (L|(L|(L|49)))));
            TEST("list_with_lists", listref2->TextRepresentation().c_str(),
                 "((25) 36 (((49))))");
            SReference listref3((L|(L|(L|(L|(L))))));
            TEST("empty_buried_list", listref3->TextRepresentation().c_str(),
                 (SString("((((")+
                  (*PTheEmptyList)->
                  TextRepresentation().c_str()+
                  SString("))))")).c_str());
            SReference dotpairref((L|25 || 36));
            TEST("dotted_pair", dotpairref->TextRepresentation().c_str(),
                 "(25 . 36)");
            SReference dotlistref((L|9, 16, 25)||36);
            TEST("dotted_list", dotlistref->TextRepresentation().c_str(),
                 "(9 16 25 . 36)");
            SReference dotpairref2((L|25)^36);
            TEST("dotted_pair", dotpairref2->TextRepresentation().c_str(),
                 "((25) . 36)");
            SReference dotlistref2((L|9, 16, 25)^36);
            TEST("dotted_list", dotlistref2->TextRepresentation().c_str(),
                 "((9 16 25) . 36)");
            SReference just_a_cons(SReference("abc")^SReference(225));
            TEST("cons_with_^", just_a_cons->TextRepresentation().c_str(),
                 "(\"abc\" . 225)");
            SReference empty(L);
            empty,25;
            TEST("comma_on_empty_list", empty->TextRepresentation().c_str(),
                 "(25)");
        }
        TestSubsection("IsEql");
        {
            SReference five(5);
            TESTB("is_eql_numbers", five.IsEql(5));            
            TESTB("isnt_eql_numbers", !five.IsEql(3));            
            TESTB("isnt_eql_num_to_string", !five.IsEql("abc"));            
            SReference abc("abc");
            TESTB("is_eql_strings", abc.IsEql("abc"));            
            TESTB("isnt_eql_strings", !abc.IsEql("def"));            
            TESTB("isnt_eql_string_to_num", !abc.IsEql(5));            
        }
        TestSubsection("UnboundByDefault");
        {
            SReference unbound;
            TESTB("sreference_unbound", !unbound.GetPtr());

            GenericSReference<MyTestTerm22, IntelibX_wrong_expression_type> p;
            TESTB("gensref_unbound", !p.GetPtr());
        }

#if 0  // no support for booleans
        TestSubsection("Booleans");
        {
            SReference true_ref(LTheLispBooleanTrue);
            TESTB("boolean_true", true_ref->IsTrue());
            SReference false_ref(LTheLispBooleanFalse);
            TESTB("boolean_false", !(false_ref->IsTrue()));
            SReference some_ref(25);
            TESTB("boolean_some", some_ref->IsTrue());

        }
#endif
        TestSubsection("Epilogue");
        TEST("final-cleanup", destructed, constructed);
        TestScore();
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    return 0;
}


