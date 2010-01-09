//   InteLib                                    http://www.intelib.org
//   The file tests/sexpress/t_sizes.cpp
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


