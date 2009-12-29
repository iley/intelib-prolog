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
#include "sexpress/sbacklnk.hpp"

#include "refal/rvars.hpp"
#include "refal/rfmatch.hpp"



void doprint(const SExpression *ref)
{
    printf("%s\n", ref ? ref->TextRepresentation().c_str() : "(unbound)");
}

void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}



RfFormConstructor R;

int main()
{
    poc();
#ifdef INTELIB_DEBUG_COUNTERS
    int before = SExpression::object_counter;
#endif
    try {
        TestSection("RefalVariables");
        TestSubsection("E");
        {
            RfVariable_E e_1("e_1");
            RfReference view((R|"ab",(R|"ef"),"ij"));
            RfReference res;
            RfContext context;
            context.AddVariable(e_1.GetPtr(), view->GetFirst(),
                view->GetLast(), 0);

            res = RefalSubstitute((R|e_1), context, false);
            TESTTR("open_e_result_1", res, "");

            context.IsBacktrackingPossible();
            res = RefalSubstitute((R|e_1), context, false);
            TESTTR("open_e_result_2", res, "\"a\"");

            context.IsBacktrackingPossible();
            res = RefalSubstitute((R|e_1), context, false);
            TESTTR("open_e_result_3", res, "\"ab\"");

            context.IsBacktrackingPossible();
            res = RefalSubstitute((R|e_1), context, false);
            TESTTR("open_e_result_4", res,
                   "\"ab\" (\"ef\")");

            context.IsBacktrackingPossible();
            res = RefalSubstitute((R|e_1), context, false);
            TESTTR("open_e_result_5", res,
                   "\"ab\" (\"ef\") \"i\"");

            context.IsBacktrackingPossible();
            res = RefalSubstitute((R|e_1), context, false);
            TESTTR("open_e_result_6", res,
                   "\"ab\" (\"ef\") \"ij\"");
        }

        TestSubsection("Memory leaks");
        {
#ifdef INTELIB_DEBUG_COUNTERS
            TEST("no_leaks", SExpression::object_counter, before);
#endif
        }
        TestScore();
    }
    catch(const IntelibX &ex)
    {
        printf("Caught IntelibX: %s\n", ex.Description());
        doprint(ex.Parameter().GetPtr());
        printf("\n");
    }
    catch(...)
    {
        printf("Something strange caught\n");
    }
    poc();
    return 0;
}


