//   InteLib                                    http://www.intelib.org
//   The file tests/sexpress/t_wrap.cpp
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
#include "sexpress/swrapper.hpp"


SListConstructor L;

struct str1 { int a; int b; str1(int x) : a(x),b(x*x) {}};

int main()
{
    try {
        TestSection("SWrapperRef");
        TestSubsection("-");
        {
            SReference l = (L| SWrapperRef<str1>(str1(5)),
                            SWrapperRef<str1>(str1(6)),
                            SWrapperRef<str1>(str1(7)));
            int sum_a = 0, sum_b = 0;
            while(!l.IsEmptyList()) {
                sum_a += SWrapperRef<str1>(l.Car())->a;
                sum_b += SWrapperRef<str1>(l.Car())->b;
                l = l.Cdr();
            }
            TEST("swrapper_a", sum_a, 18);
            TEST("swrapper_b", sum_b, 25+36+49);
        }
        TestScore();
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    return 0;
}


