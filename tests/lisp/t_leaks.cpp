//   InteLib                                    http://www.intelib.org
//   The file tests/lisp/t_leaks.cpp
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

#include "lisp/lisp.hpp"
#include "lisp/lsymbol.hpp"
#include "lisp/lcont.hpp"
#include "lisp/llambda.hpp"


void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}

LListConstructor L;


#define TestTxRep(name, res, pattern) \
      Test(name, res->TextRepresentation().c_str(), pattern)


int main()
{
#ifdef INTELIB_DEBUG_COUNTERS
    poc();
    try {
        TestSection("Memory Leaks");
        TestSubsection("A.SetDynamicValue(A)");
        {
            int before = SExpression::object_counter;
            {
                LSymbol A("A");
                LSymbol B("B");
                A->SetDynamicValue(B);
            }
            int after = SExpression::object_counter;
            TEST("no_leak", after, before);
        }
        TestSubsection("A->Setq(L|A)");
        {
            int before = SExpression::object_counter;
            {
                LSymbol A("A");
                LSymbol B("B");
                A->SetDynamicValue((L|B, 1));
            }
            int after = SExpression::object_counter;
            TEST("no_leak", after, before);
        }
        TestSubsection("2");
        {
            int before = SExpression::object_counter;
            {
                LSymbol A("A");
                LSymbol B("B");
                LReference ref = (L|B, 1);
                A->SetDynamicValue(ref);
            }
            int after = SExpression::object_counter;
            TEST("no_leak", after, before);
        }
        TestSubsection("3");
        {
            int before = SExpression::object_counter;
            {
                LSymbol A("A");
                LSymbol B("B");
                LReference ref = (L|B, 1);
                A->SetDynamicValue(ref);
            }
            int after = SExpression::object_counter;
            TEST("no_leak", after, before);
        }
        TestSubsection("just the same");
        {
            int before = SExpression::object_counter;
            {
                LReference ref(1);
                ref = L|ref;
            }
            int after = SExpression::object_counter;
            TEST("no_leak", after, before);
        }
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
    poc();
#else
    printf("WARNING: The 'Memory Leaks' test set skipped\n");
#endif
    return 0;
}


