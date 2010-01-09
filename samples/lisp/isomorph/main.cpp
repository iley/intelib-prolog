//   InteLib                                    http://www.intelib.org
//   The file samples/lisp/isomorph/main.cpp
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
#include <stdlib.h>
#include <intelib/sexpress/sexpress.hpp>
#include <intelib/sexpress/sstring.hpp>
#include <intelib/lisp/lsymbol.hpp>

extern LSymbol ISOMORPHIC;
void LispInit_isomorphic();

static LListConstructor L;

void call_isomorph(const SReference &l1, const SReference &l2)
{
    SReference res = (L|ISOMORPHIC, ~l1, ~l2).Evaluate();
    printf("%s ~~ %s : %s\n",
           l1->TextRepresentation().c_str(),
           l2->TextRepresentation().c_str(),
           res->TextRepresentation().c_str());
}

int main(int argc, char *argv[])
{
    LispInit_isomorphic();
    SReference ls1 = (L|(L|1, 2), 3, 4);             // ((1 2) 3 4) 
    SReference ls2 = (L|(L|"a", "b"), "c", "d");     // (("a" "b") "c" "d") 
    SReference ls3 = (L|(L|1, 2), (L|3, 4));         // ((1 2) (3 4)) 
    call_isomorph(ls1, ls2);
    call_isomorph(ls1, ls3);
    return 0;
}
