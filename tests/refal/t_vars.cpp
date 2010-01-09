//   InteLib                                    http://www.intelib.org
//   The file tests/refal/t_vars.cpp
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


