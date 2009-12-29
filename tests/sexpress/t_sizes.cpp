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

int main()
{
    try {
        TestSection("ObjectSizes");
        TestSubsection("ObjectSizes");
        {
            TEST("sizeof_lreference", sizeof(SReference), sizeof(SExpression*));
            TEST("sizeof_sstring", sizeof(SString), sizeof(SExpression*));
        }
        TestScore();
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    return 0;
}


