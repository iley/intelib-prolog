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


