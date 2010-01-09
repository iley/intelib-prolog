//   InteLib                                    http://www.intelib.org
//   The file tests/sexpress/t_ix.cpp
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

class MyIntelibX : public IntelibX {
public:
    MyIntelibX(SReference comment) : IntelibX("MyIntelibX", comment) {}
    ~MyIntelibX() {}
};

int main()
{
    try {
        TestSection("IntelibExceptions");
        TestSubsection("IntelibX");
        {
            int flag = 0;
            const char *p = "WRONG";
            SReference param("WRONG_PARAM");
            try {
                throw MyIntelibX("generic");
            }
            catch(const IntelibX &ex) {
                flag = 0x55;
                p = ex.Description();
                param = ex.Parameter();
            }
            TEST("catch_lispx", flag, 0x55);
            TEST("lispx_comment", p, "MyIntelibX");
            TEST("lispx_parameter", param->TextRepresentation().c_str(),
                 "\"generic\"");
        }
#if 0 // no stack support in sexpression version
        TestSubsection("IntelibX_stack");
        {
            int check = 0;
            SReference stack;
            try{
                try{
                    try{
                        try{
                            throw MyIntelibX("99");
                        }
                        catch(IntelibX &ex)
                        {
                            check += 1;
                            ex.AddStack(25);
                            throw ex;
                        }
                    }
                    catch(IntelibX &ex)
                    {
                        check += 20;
                        ex.AddStack("fun");
                        throw ex;
                    }
                }
                catch(IntelibX &ex)
                {
                    check += 300;
                    ex.AddStack(49);
                    throw ex;
                }
            }
            catch(const IntelibX &ex)
            {
                check += 4000;
                stack = ex.Stack();
            }
            TEST("lispx_stack_flow", check, 4321);
            TEST("lispx_stack", stack->TextRepresentation().c_str(),
                 "(49 \"fun\" 25)");
        }
#endif
        TestSubsection("Intelib_assert");
        {
            int flag = 0x001;
            try {
                int a = 1;
                int b = 2;
                INTELIB_ASSERT(a>b, MyIntelibX(222));
                printf("Uncaught\n");
                flag |= 0x010;
            }
            catch(const IntelibX &ex)
            {
                printf("Caught\n");
                flag |= 0x100;
            }
      #if INTELIB_RUNTIME_CHECKS == 1
            TEST("intelib_assert", flag, 0x101);
      #else
            TEST("intelib_assert", flag, 0x011);
      #endif
        }
        TestScore();
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    return 0;
}


