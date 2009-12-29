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
#include "refal/refal.hpp"

RfFormConstructor R;

void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}

int main()
{
    try {
        TestSection("RfReference");
#ifdef INTELIB_DEBUG_COUNTERS
        int before = SExpression::object_counter;
#endif
        TestSubsection("InsertAfter");
        {
            RfNodeRef i(new RfExpressionNode(1, *PTheEmptyList));
            Test("one_element_a", i->TextRepresentation().c_str(),
                 "(1)");
            i.InsertAfter(2);
            Test("insert_after", i->TextRepresentation().c_str(), "(1 2)");
            i.InsertAfter(3);
            i.InsertAfter(4);
            i.InsertAfter(5);
            Test("begin_after", i->TextRepresentation().c_str(), "(1 5 4 3 2)");
        }
        TestSubsection("ListConstruction");
        {
            RfNodeRef br,brr;
            brr = (br,1,2,3,4,5);
            Test("comma_returns_end", brr->TextRepresentation().c_str(), "(5)");
            Test("comma_doesnt_advance", br->TextRepresentation().c_str(),
                 "(1 2 3 4 5)");
            RfReference r((R|1,2,3,4,5));
            Test("comma_construction", r->TextRepresentation().c_str(),
                 "1 2 3 4 5");
        }
        TestSubsection("Navigation");
        {
            RfReference lr((R|1,2,3,4,5));
            RfNodeRef r(lr->GetFirst());

            r++;
            Test("increment", r->TextRepresentation().c_str(), "(2 3 4 5)");

            r = lr->GetLast();
            r--;
            Test("decrement", r->TextRepresentation().c_str(), "(4 5)");

            r = lr->GetFirst();
            r++;
            Test("prev", r->Prev()->TextRepresentation().c_str(), "(1 2 3 4 5)");
            Test("next", r->Next()->TextRepresentation().c_str(), "(3 4 5)");

        }
        TestSubsection("AutoDisconnect");
        {
            RfNodeRef r1 = new RfExpressionNode(1, *PTheEmptyList);
            RfNodeRef r2 = new RfExpressionNode(2, *PTheEmptyList);
            r2->ConnectBack(r1);
            Test("connected", r2->Prev()->TextRepresentation().c_str(), "(1 2)");
            Test("connected_yes", r1->TextRepresentation().c_str(), "(1 2)");
            r1 = *PTheEmptyList;
            Test("disconnected", r2->Prev().IsEmptyList());
        }
        TestSubsection("Exclude");
        {
            RfReference l((R|1,2,3,4,5,6,7,8,9));
            RfNodeRef left = l->GetFirst();
            RfNodeRef right = l->GetLast();
            left++;
            left++;
            left++;
            right--;
            right--;
            right--;
            l->Exclude(left, right);
            Test("exclude_from_middle", l->TextRepresentation().c_str(),
                 "1 2 3 7 8 9");
            Test("excluded_from_middle_lives", left->TextRepresentation().c_str(),
                 "(4 5 6)");

            left = l->GetFirst();
            right = l->GetFirst();
            right++;
            right++;
            l->Exclude(left, right);
            Test("exclude_from_middle", l->TextRepresentation().c_str(),
                 "7 8 9");
            right = l->GetLast();
            left = l->GetFirst();
            left++;
            l->Exclude(left, right);
            Test("exclude_from_middle", l->TextRepresentation().c_str(),
                 "7");
        }
        TestSubsection("InsertListAfter");
        {
            RfReference l((R|10,20,30));
            RfReference ls1((R|1,2,3));
            RfNodeRef left = ls1->GetFirst();
            RfNodeRef right = ls1->GetLast();
            ls1->Exclude(left, right);

            RfNodeRef from = l->GetLast();
            l->InsertListAfter(from, left, right);
            Test("insert_list_into_end0", l->TextRepresentation().c_str(),
                 "10 20 30 1 2 3");
        }
        {
            RfReference l(new RfExpression);
            RfReference ls1((R|1,2,3));
            RfNodeRef left = ls1->GetFirst();
            RfNodeRef right = ls1->GetLast();
            ls1->Exclude(left, right);

            l->InsertListAfter(*PTheEmptyList, left, right);
            Test("insert_list_into_empty", l->TextRepresentation().c_str(),
                 "1 2 3");

            ls1 = (R|11,22,33);
            left = ls1->GetFirst();
            right = ls1->GetLast();
            ls1->Exclude(left, right);
            l->InsertListAfter(*PTheEmptyList, left, right);
            Test("insert_list_at_begin", l->TextRepresentation().c_str(),
                 "11 22 33 1 2 3");

            RfReference ls3((R|10,20,30));
            left = ls3->GetFirst();
            right = ls3->GetLast();
            ls3->Exclude(left, right);
            l->InsertListAfter(l->GetLast(), left, right);
            Test("insert_list_at_end", l->TextRepresentation().c_str(),
                 "11 22 33 1 2 3 10 20 30");


            RfNodeRef from = l->GetLast();

            //        printf(" **** last: %s", from->TextRepresentation().c_str());

            from--;
            from--;
            from--;

            ls1 = (R|9,8,7);
            left = ls1->GetFirst();
            right = ls1->GetLast();
            ls1->Exclude(left, right);
            l->InsertListAfter(from, left, right);
            Test("insert_list_in_middle", l->TextRepresentation().c_str(),
                 "11 22 33 1 2 3 9 8 7 10 20 30");
        }

        TestSubsection("Ranges");
        {
            RfReference ll((R|1,2,3,4,5,6,7,8,9,10));
            RfRange r(ll);
            Test("whole_list", r.Copy()->TextRepresentation().c_str(),
                 "1 2 3 4 5 6 7 8 9 10");
            for(int i=0; i<3; i++) r.LeftEnd()++;
            Test("left_end_move_right", r.Copy()->TextRepresentation().c_str(),
                 "4 5 6 7 8 9 10");
            for(int i=0; i<3; i++) r.RightEnd()--;
            Test("right_end_move_left", r.Copy()->TextRepresentation().c_str(),
                 "4 5 6 7");

            Test("get_first", r.GetFirst()->Car()->TextRepresentation().c_str(),
                 "4");
            Test("get_last", r.GetLast()->Car()->TextRepresentation().c_str(),
                 "7");
            Test("get_follow", r.GetFollow()->Car()->TextRepresentation().c_str(),
                 "8");

            Test("whats_before",
                 r.WhatsBefore().Copy()->TextRepresentation().c_str(),
                 "1 2 3");

            Test("whats_after",
                 r.WhatsAfter().Copy()->TextRepresentation().c_str(),
                 "8 9 10");

            for(int i=0; i<4; i++) r.RightEnd()--;
            Test("right_end_moved_left_to_limit",
                 r.Copy()->TextRepresentation().c_str(),
                 "");
            r.RightEnd()--;
            r.RightEnd()--;
            r.LeftEnd()--;
            Test("right_end_moved_over_limit_moves_left_end",
                 r.Copy()->TextRepresentation().c_str(),
                 "1");
            r.LeftEnd()++;
            r.LeftEnd()++;
            r.LeftEnd()++;
            r.RightEnd()++;
            Test("left_end_moved_over_limit_moves_right_end",
                 r.Copy()->TextRepresentation().c_str(),
                 "4");
            r.RightEnd()--;
            r.RightEnd()--;
            Test("_____empty", r.IsEmpty());
            Test("get_first_when_empty", r.GetFirst().IsEmptyList());
            Test("get_last_when_empty", r.GetLast().IsEmptyList());

        }
#ifdef INTELIB_DEBUG_COUNTERS
        int after = SExpression::object_counter;
        TestSubsection("Backlink memory");
        {
            Test("no_leak", after, before);
        }
#endif
        TestScore();
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    return 0;
}


