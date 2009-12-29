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

#include "sexpress/sexpress.hpp"
#include "sexpress/iexcept.hpp"
#include "sexpress/sstring.hpp"

void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}

SListConstructor L;


int main()
{
#ifdef INTELIB_DEBUG_COUNTERS
    poc();
    try {
        TestSection("Memory Leaks");
#if 0
        TestSubsection("A.Setq(A)");
        {
            int before = SExpression::object_counter;
            {
                LSymbol A("A");
                A.Setq(A);
            }
            int after = SExpression::object_counter;
            TEST("no_leak", after, before);
        }
        TestSubsection("A.Setq(L|A)");
        {
            int before = SExpression::object_counter;
            {
                LSymbol A("A");
                A.Setq((L|A, 1));
            }
            int after = SExpression::object_counter;
            TEST("no_leak", after, before);
        }
        TestSubsection("2");
        {
            int before = SExpression::object_counter;
            {
                LSymbol A("A");
                SReference ref = (L|A, 1);
                A.Setq(ref);
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
                SReference ref = (L|B, 1);
                A.Setq(ref);
            }
            int after = SExpression::object_counter;
            TEST("no_leak", after, before);
        }
#endif
        TestSubsection("just the same");
        {
            int before = SExpression::object_counter;
            {
                SReference ref = 1;
                ref = L|ref;
            }
            int after = SExpression::object_counter;
            TEST("no_leak", after, before);
        }
        TestScore();
    }
    catch(const IntelibX &ex) {
        printf("Caught IntelibX: %s\n%s\n", ex.Description(),
                ex.Parameter()->TextRepresentation().c_str());
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    poc();
    //LTheDefaultThread.ResetReturn();
    poc();
#else
    printf("WARNING: The 'Memory Leaks' test set skipped\n");
#endif
    return 0;
}


