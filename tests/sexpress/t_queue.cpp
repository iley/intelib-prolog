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
#include "sexpress/squeue.hpp"


void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}


int main()
{
    poc();
    try {
        TestSection("SQueue");
        TestSubsection("Creation");
        {
            SQueue q;
            q|1,2,3,4,"five",6;
            TESTTR("queue_created", q, "(1 2 3 4 \"five\" 6)");
        }
        TestSubsection("Insertion");
        {
            SQueue q;
            q|1,2,3,4,"five",6;
            q.InsertFirst(25);
            TESTTR("insert_first", q, "(25 1 2 3 4 \"five\" 6)");
            q.Append(36);
            TESTTR("append_still_works", q, "(25 1 2 3 4 \"five\" 6 36)");
        }
        TestSubsection("RemoveFirst");
        {
            SQueue q;
            q|1,2,3,4,"five",6;
            q.RemoveFirst();
            TESTTR("remove_first", q, "(2 3 4 \"five\" 6)");
            q.RemoveFirst();
            q.RemoveFirst();
            q.RemoveFirst();
            q.RemoveFirst();
            TESTTR("remove_all_but_last", q, "(6)");
            q.RemoveFirst();
            TEST("removed_last", q.IsEmpty(), true);
        }
        TestScore();
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    poc();
    return 0;
}


