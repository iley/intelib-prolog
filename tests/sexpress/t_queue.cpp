//   InteLib                                    http://www.intelib.org
//   The file tests/sexpress/t_queue.cpp
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


