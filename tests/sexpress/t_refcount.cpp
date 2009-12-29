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

#include "sexpress/refcount.hpp"


int constructed = 0;
int destructed = 0;

class MyTestClass : public GarbageSafe {
    int i;
public:
    MyTestClass() : i(0) { constructed ++; }
    void Set(int ai) { i = ai; }
    int Get() const { return i; }
protected:
    ~MyTestClass() { destructed ++; }
};

typedef GenericReference<MyTestClass> MyPtr;

void Reset()
{
    constructed = destructed = 0;
}

//#define NULL 0

int main()
{
    try {
        TestSection("Garbage Collection");




        TestSubsection("GarbageSafe");
        {
            GarbageSafePointer p1;
            p1 = new MyTestClass;
            p1 = 0;
            TESTB("destructed-after-null-assignment",
                 destructed == 1 && constructed == 1);
            Reset();
            GarbageSafePointer p2(new MyTestClass);
            GarbageSafePointer p3;
            p3 = p2;
            TESTB("equality-true", p2 == p3);
            TESTB("non-equality-false", !(p2 != p3));
            TESTB("single-object", constructed == 1 && destructed == 0);
            p2 = 0;
            TEST("assigned-pointer-keeps", destructed, 0);
            p3 = 0;
            TEST("last-reference-kills", destructed, 1);
        }



        TestSubsection("GenericReference");
        {
            Reset();
            MyPtr p1(new MyTestClass);
            TESTB("single-object", constructed == 1 && destructed == 0);
            p1->Set(55);
            TEST("getting", p1->Get(), 55);
            MyPtr cp1(p1);
            TESTB("two-pointers-to-the-same",
                      constructed == 1 && destructed == 0);
            TEST("const-getting", cp1->Get(), 55);
            p1=0;
            TESTB("first-pointer-cleared", constructed == 1 && destructed == 0);
            cp1=0;
            TESTB("second-pointer-cleared",
                      constructed == 1 && destructed == 1);
            Reset();
            p1 = new MyTestClass;
            cp1 = p1;
            MyPtr p2(new MyTestClass);
            MyPtr cp2(p2);
            MyPtr p3(p2);
            MyPtr cp3(p3);
            MyPtr p4;
            MyPtr cp4;
            TESTB("two-objects", constructed == 2 && destructed == 0);
            TESTB("p1-neq-p2", p1 != p2);
            TESTB("p2-neq-p1", p2 != p1);
            TESTB("n-p1-neq-p2", !(p1 == p2));
            TESTB("n-p2-neq-p1", !(p2 == p1));
            TESTB("cp1-neq-cp2", cp1 != cp2);
            TESTB("cp2-neq-cp1", cp2 != cp1);
            TESTB("n-cp1-neq-cp2", !(cp1 == cp2));
            TESTB("n-cp2-neq-cp1", !(cp2 == cp1));
            TESTB("equality-n-c", p1 == cp1);
            TESTB("equality-c-n", cp1 == p1);
            TESTB("equality-n-n", p3 == p2);
            TESTB("equality-c-c", cp3 == cp2);
            TESTB("non-equality-n-c", p2 != cp1);
            TESTB("non-equality-c-n", cp1 != p2);
            TESTB("non-equality-n-n", p3 != p1);
            TESTB("non-equality-c-c", cp3 != cp1);
            TESTB("failed-equality-n-c", !(p2 == cp1));
            TESTB("failed-equality-c-n", !(cp1 == p2));
            TESTB("failed-equality-n-n", !(p3 == p1));
            TESTB("failed-equality-c-c", !(cp3 == cp1));
            TESTB("failed-non-equality-n-c", !(p1 != cp1));
            TESTB("failed-non-equality-c-n", !(cp1 != p1));
            TESTB("failed-non-equality-n-n", !(p3 != p2));
            TESTB("failed-non-equality-c-c", !(cp3 != cp2));
            TESTB("still-two-objects", constructed == 2 && destructed == 0);
            //TESTB("equality-with-null-n", p4 == 0);
            //TESTB("equality-with-null-c", cp4 == 0);
            //TESTB("equality-with-null--n", 0 == p4);
            //TESTB("equality-with-null--c", 0 == cp4);
            //TESTB("still-still-two-objects",
                  //constructed == 2 && destructed == 0);
            //TESTB("pointer-is-true", p1);
            //TESTB("const-pointer-is-true", cp1);
            //TESTB("null-pointer-is-not-true", !p4);
            //TESTB("null-const-pointer-is-not-true", !cp4);
            //TESTB("null-pointer-is-false", p4 ? 0 : 1);
            //TESTB("null-const-pointer-is-false", cp4 ? 0 : 1);
        }
        {
            Reset();
            MyPtr p1;
            MyPtr nullptr;
            p1 = new MyTestClass;
            TESTB("assign_of_null", p1 != nullptr);
            MyPtr p2(new MyTestClass);
            MyPtr p3;
            TESTB("p3_is_null", p3 == nullptr);
            p3 = p2;
            TESTB("assign_of_null2", p3 != nullptr);

        }

        TestScore();
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    return 0;
}


