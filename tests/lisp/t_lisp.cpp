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
#include <string.h>
#include "tests.hpp"

#include "sexpress/sstring.hpp"
#include "sexpress/iexcept.hpp"

#include "lisp/lisp.hpp"

int constructed = 0;
int destructed = 0;

class MyTestTerm : public SExpression {
    int i;
public:
    static IntelibTypeId TypeId;
    MyTestTerm(int a) : SExpression(TypeId), i(a) { constructed ++; }
    ~MyTestTerm() { destructed ++; }
    int Get() const { return i; }

    SString TextRepresentation() const { return "---"; }
};

class MyTestTerm2 : public SExpression {
    int i;
public:
    static IntelibTypeId TypeId;
    MyTestTerm2(int a) : SExpression(TypeId), i(a) { constructed ++; }
    ~MyTestTerm2() { destructed ++; }
    int Get() const { return i; }

    SString TextRepresentation() const { return "---"; }
};

class MyTestTerm22 : public SExpression {
    int i;
public:
    static IntelibTypeId TypeId;
    MyTestTerm22(int a) : SExpression(TypeId), i(a) { constructed ++; }
    ~MyTestTerm22() { destructed ++; }
    int Get() const { return i; }

    SString TextRepresentation() const { return "---"; }
};

IntelibTypeId MyTestTerm::TypeId(&SExpression::TypeId);
IntelibTypeId MyTestTerm2::TypeId(&SExpression::TypeId);
IntelibTypeId MyTestTerm22::TypeId(&MyTestTerm2::TypeId);


int main()
{
    try {
        TestSection("StdTerms");
        TestSubsection("SExpressionLabel");
        {
            SLabel lab1("lab1");
            LReference labref(lab1);
            TEST("label", labref->TextRepresentation().c_str(),
                 "lab1");
            TESTB("label_equality", labref == LReference(lab1));
            LReference labref2(lab1);
            TESTB("label_equality2", labref == labref2);
            LReference ref3(25);
            TESTB("label_non_eq", labref != ref3);

        }
        TestSubsection("SListConstructor");
        {
            SListConstructor L;
            LReference list_int((L|25));
            TEST("list_of_1_int", list_int->TextRepresentation().c_str(),
                 "(25)");
            LReference list_str((L|"abcd"));
            TEST("list_of_1_str", list_str->TextRepresentation().c_str(),
                 "(\"abcd\")");
            intelib_float_t fl = 1.1;
            LReference list_float((L|fl));
            char buf[128];
            snprintf(buf, sizeof(buf), "(" INTELIB_FLOAT_FORMAT ")", fl);
            TEST("list_of_1_float", list_float->TextRepresentation().c_str(),
                 buf);
        }
        TestSubsection("ListConstructionAlgebra");
        {
            SListConstructor L;
            LReference listref((L|25, 36, 49, "abcd", "efgh"));
            TEST("plain_list", listref->TextRepresentation().c_str(),
                 "(25 36 49 \"abcd\" \"efgh\")");
            LReference listref2((L|(L|25), 36, (L|(L|(L|49)))));
            TEST("list_with_lists", listref2->TextRepresentation().c_str(),
                 "((25) 36 (((49))))");
            LReference listref3((L|(L|(L|(L|(L))))));
            TEST("empty_buried_list", listref3->TextRepresentation().c_str(),
                 (SString("((((")+
                  (*PTheEmptyList)->
                  TextRepresentation().c_str()+
                  SString("))))")).c_str());
            LReference dotpairref((L|25 || 36));
            TEST("dotted_pair", dotpairref->TextRepresentation().c_str(),
                 "(25 . 36)");
            LReference dotlistref((L|9, 16, 25)||36);
            TEST("dotted_list", dotlistref->TextRepresentation().c_str(),
                 "(9 16 25 . 36)");
            LReference dotpairref2((L|25)^36);
            TEST("dotted_pair", dotpairref2->TextRepresentation().c_str(),
                 "((25) . 36)");
            LReference dotlistref2((L|9, 16, 25)^36);
            TEST("dotted_list", dotlistref2->TextRepresentation().c_str(),
                 "((9 16 25) . 36)");
            LReference just_a_cons(LReference("abc")^LReference(225));
            TEST("cons_with_^", just_a_cons->TextRepresentation().c_str(),
                 "(\"abc\" . 225)");
            LReference empty(L);
            empty,25;
            TEST("comma_on_empty_list", empty->TextRepresentation().c_str(),
                 "(25)");
        }
        TestSubsection("Booleans");
        {
            LReference true_ref(*PTheLispBooleanTrue);
            TESTB("boolean_true", true_ref.IsTrue());
            LReference false_ref(*PTheLispBooleanFalse);
            TESTB("boolean_false", !(false_ref.IsTrue()));
            LReference some_ref(25);
            TESTB("boolean_some", some_ref.IsTrue());

        }
        TestSubsection("TextRepresentaiton");
        {
            LListConstructor L;
            LReference qref = ~LReference(25);
            TEST("quote_text_representation",
                 qref.TextRepresentation().c_str(), "'25");
        }
        TestSubsection("Quotation");
        {
            SListConstructor S;
            LReference ref = ~(S|1,2,3);
            TEST("s_quotation", ref.TextRepresentation().c_str(), "'(1 2 3)");
        }
        TestSubsection("Epilogue");
        TEST("final-cleanup", destructed, constructed);
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


