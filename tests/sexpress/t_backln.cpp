//   InteLib                                    http://www.intelib.org
//   The file tests/sexpress/t_backln.cpp
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
#include "sexpress/sbacklnk.hpp"
#include "sexpress/sdbllist.hpp"

SDoubleListConstructor D;

void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}

int main()
{
    try {
        TestSection("SDoubleList");
#ifdef INTELIB_DEBUG_COUNTERS
        int before = SExpression::object_counter;
#endif
        TestSubsection("Begins&Ends");
        {
            SDoubleList lr;
            SExpressionDoubleList::Iterator i(lr);
            TESTB("exhausted", i.Exhausted());
            TESTB("exhausted_not_begin", !i.IsBegin());
            TESTB("exhausted_not_end", !i.IsEnd());
            i.InsertBefore(1);
            TESTB("not_exhausted", !i.Exhausted());
            TESTB("one_is_begin", i.IsBegin());
            TESTB("one_is_end", i.IsEnd());
            i.InsertBefore(2);
            TESTB("no_longer_begin", !i.IsBegin());
            TESTB("still_end", i.IsEnd());
            i.InsertAfter(3);
            TESTB("no_longer_end", !i.IsEnd());
        }
        TestSubsection("InsertBefore");
        {
            SDoubleList lr;
            SExpressionDoubleList::Iterator i(lr);
            i.InsertBefore(1);
            TEST("one_element", lr->TextRepresentation().c_str(), "(1)");
            i.InsertBefore(2);
            TEST("insert_before", lr->TextRepresentation().c_str(), "(2 1)");
            i.InsertBefore(3);
            i.InsertBefore(4);
            i.InsertBefore(5);
            TEST("begin", lr->TextRepresentation().c_str(), "(2 3 4 5 1)");
        }
        TestSubsection("InsertAfter");
        {
            SDoubleList lr;
            SExpressionDoubleList::Iterator i(lr);
            i.InsertAfter(1);
            TEST("one_element_a", i.Begin()->TextRepresentation().c_str(),
                 "(1)");
            i.InsertAfter(2);
            TEST("insert_after", lr->TextRepresentation().c_str(), "(1 2)");
            i.InsertAfter(3);
            i.InsertAfter(4);
            i.InsertAfter(5);
            TEST("begin_after", lr->TextRepresentation().c_str(), "(1 5 4 3 2)");
        }
        TestSubsection("ListConstruction");
        {
            SBacklinkRef br,brr;
            brr = (br,1,2,3,4,5);
            TEST("comma_returns_end", brr->TextRepresentation().c_str(), "(5)");
            TEST("comma_doesnt_advance", br->TextRepresentation().c_str(),
                 "(1 2 3 4 5)");
            SDoubleListRef r((D|1,2,3,4,5));
            TEST("comma_construction", r->TextRepresentation().c_str(),
                 "(1 2 3 4 5)");
        }
        TestSubsection("RemovingElement");
        {
            SDoubleListRef lr((D|1,2,3,4,5));
            SExpressionDoubleList::Iterator r(lr, true);
            r--; r--;
            r.Remove();
            TEST("remove_moves_backwards", r->TextRepresentation().c_str(),
                 "(2 4 5)");

            TEST("remove", lr->TextRepresentation().c_str(),  "(1 2 4 5)");
            r.SetBegin();
            r.Remove();
            TEST("removed_from_begin", lr->TextRepresentation().c_str(),
                 "(2 4 5)");
            r.SetEnd();
            r.Remove();
            TEST("removed_from_end", lr->TextRepresentation().c_str(),
                 "(2 4)");
        }
        TestSubsection("Navigation");
        {
            SDoubleListRef lr((D|1,2,3,4,5));
            SExpressionDoubleList::Iterator r(lr, true);
            SBacklinkRef p;

            r.SetBegin();
            p = ++r;
            TEST("preincrement", r->TextRepresentation().c_str(), "(2 3 4 5)");
            TEST("preinc_val", p->TextRepresentation().c_str(), "(2 3 4 5)");

            r.SetBegin();
            p = r++;
            TEST("postincrement", r->TextRepresentation().c_str(), "(2 3 4 5)");
            TEST("postinc_val", p->TextRepresentation().c_str(), "(1 2 3 4 5)");

            r.SetEnd();
            p = --r;
            TEST("predecrement", r->TextRepresentation().c_str(), "(4 5)");
            TEST("predec_val", p->TextRepresentation().c_str(), "(4 5)");

            r.SetEnd();
            p = r--;
            TEST("postdecrement", r->TextRepresentation().c_str(), "(4 5)");
            TEST("postdec_val", p->TextRepresentation().c_str(), "(5)");

            SDoubleList ll;
            SExpressionDoubleList::Iterator l(ll);
            l.InsertBefore(2);
            l.InsertBefore(1);
            l.InsertAfter(3);

            TEST("prev", l->Prev()->TextRepresentation().c_str(), "(1 2 3)");
            TEST("next", l->Next()->TextRepresentation().c_str(), "(3)");

        }
        TestSubsection("AutoDisconnect");
        {
            SBacklinkRef r1 = new SExpressionBacklink(1, *PTheEmptyList);
            SBacklinkRef r2 = new SExpressionBacklink(2, *PTheEmptyList);
            r2->ConnectBack(r1);
            TEST("connected", r2->Prev()->TextRepresentation().c_str(), "(1 2)");
            r1 = *PTheEmptyList;
            TESTB("disconnected", r2->Prev().IsEmptyList());
        }
        TestSubsection("Exclude");
        {
            SDoubleListRef l((D|1,2,3,4,5,6,7,8,9));
            SBacklinkRef left = l->GetBegin();
            SBacklinkRef right = l->GetEnd();
            left++;
            left++;
            left++;
            right--;
            right--;
            right--;
            l->Exclude(left, right);
            TEST("exclude_from_middle", l->TextRepresentation().c_str(),
                 "(1 2 3 7 8 9)");
            TEST("excluded_from_middle_lives", left->TextRepresentation().c_str(),
                 "(4 5 6)");

            left = l->GetBegin();
            right = l->GetBegin();
            right++;
            right++;
            l->Exclude(left, right);
            TEST("exclude_from_middle", l->TextRepresentation().c_str(),
                 "(7 8 9)");
            right = l->GetEnd();
            left = l->GetBegin();
            left++;
            l->Exclude(left, right);
            TEST("exclude_from_middle", l->TextRepresentation().c_str(),
                 "(7)");
        }
        TestSubsection("InsertListAfter");
        {
            SDoubleListRef l(new SExpressionDoubleList);
            SDoubleListRef ls1((D|1,2,3));
            SBacklinkRef left = ls1->GetBegin();
            SBacklinkRef right = ls1->GetEnd();
            ls1->Exclude(left, right);

            l->InsertListAfter(*PTheEmptyList, left, right);
            TEST("insert_list_into_empty", l->TextRepresentation().c_str(),
                 "(1 2 3)");

            ls1 = (D|11,22,33);
            left = ls1->GetBegin();
            right = ls1->GetEnd();
            ls1->Exclude(left, right);
            l->InsertListAfter(*PTheEmptyList, left, right);
            TEST("insert_list_at_begin", l->TextRepresentation().c_str(),
                 "(11 22 33 1 2 3)");

            ls1 = (D|10,20,30);
            left = ls1->GetBegin();
            right = ls1->GetEnd();
            ls1->Exclude(left, right);
            l->InsertListAfter(l->GetEnd(), left, right);
            TEST("insert_list_at_begin", l->TextRepresentation().c_str(),
                 "(11 22 33 1 2 3 10 20 30)");

            SBacklinkRef from = l->GetEnd();
            from--;
            from--;
            from--;

            ls1 = (D|9,8,7);
            left = ls1->GetBegin();
            right = ls1->GetEnd();
            ls1->Exclude(left, right);
            l->InsertListAfter(from, left, right);
            TEST("insert_list_in_middle", l->TextRepresentation().c_str(),
                 "(11 22 33 1 2 3 9 8 7 10 20 30)");

        }

        TestSubsection("Ranges");
        {
            SDoubleListRef ll((D|1,2,3,4,5,6,7,8,9,10));
            SDoubleListRange r(ll);
            TEST("whole_list", r.Copy()->TextRepresentation().c_str(),
                 "(1 2 3 4 5 6 7 8 9 10)");
            for(int i=0; i<3; i++) r.LeftEnd()++;
            TEST("left_end_move_right", r.Copy()->TextRepresentation().c_str(),
                 "(4 5 6 7 8 9 10)");
            for(int i=0; i<3; i++) r.RightEnd()--;
            TEST("right_end_move_left", r.Copy()->TextRepresentation().c_str(),
                 "(4 5 6 7)");

            TEST("get_first", r.GetFirst()->Car()->TextRepresentation().c_str(),
                 "4");
            TEST("get_last", r.GetLast()->Car()->TextRepresentation().c_str(),
                 "7");
            TEST("get_follow", r.GetFollow()->Car()->TextRepresentation().c_str(),
                 "8");

            TEST("whats_before",
                 r.WhatsBefore().Copy()->TextRepresentation().c_str(),
                 "(1 2 3)");

            TEST("whats_after",
                 r.WhatsAfter().Copy()->TextRepresentation().c_str(),
                 "(8 9 10)");

            for(int i=0; i<4; i++) --r.RightEnd();
            TEST("right_end_moved_left_to_limit",
                 r.Copy()->TextRepresentation().c_str(),
                 "()");
            r.RightEnd()--;
            r.RightEnd()--;
            r.LeftEnd()--;
            TEST("right_end_moved_over_limit_moves_left_end",
                 r.Copy()->TextRepresentation().c_str(),
                 "(1)");
            ++r.LeftEnd();
            ++r.LeftEnd();
            ++r.LeftEnd();
            ++r.RightEnd();
            TEST("left_end_moved_over_limit_moves_right_end",
                 r.Copy()->TextRepresentation().c_str(),
                 "(4)");
            r.LeftEnd().JumpToFirst();
            TEST("left_end_jump_to_begin",
                 r.Copy()->TextRepresentation().c_str(),
                 "(1 2 3 4)");
            r.RightEnd().JumpToLast();
            TEST("right_end_jump_to_end",
                 r.Copy()->TextRepresentation().c_str(),
                 "(1 2 3 4 5 6 7 8 9 10)");
            r.RightEnd()--;
            r.RightEnd()--;
            r.LeftEnd().JumpToLast();
            TESTB("left_end_jump_to_end_exhausted", r.Exhausted());
            TESTB("left_end_jump_to_end_empty", r.IsEmpty());
            r.LeftEnd()--;
            TEST("left_end_jump_to_end",
                 r.Copy()->TextRepresentation().c_str(),
                 "(10)");
            r.RightEnd().JumpToFirst();
            TESTB("right_end_jump_to_end_not_exhusted", !r.Exhausted());
            TESTB("right_end_jump_to_begin_empty", r.IsEmpty());
            r.RightEnd()++;
            TEST("right_end_jump_to_begin",
                 r.Copy()->TextRepresentation().c_str(),
                 "(1)");

            r.RightEnd().JumpToLast();
            TEST("get_last_at_end",
                 r.GetLast()->Car()->TextRepresentation().c_str(), "10");

            r.LeftEnd().JumpToLast();
            r.RightEnd()--;
            r.RightEnd()--;
            TESTB("_____empty", r.IsEmpty());
            TESTB("get_first_when_empty", r.GetFirst().IsEmptyList());
            TESTB("get_last_when_empty", r.GetLast().IsEmptyList());

        }
#ifdef INTELIB_DEBUG_COUNTERS
        int after = SExpression::object_counter;
        TestSubsection("Backlink memory");
        {
            TEST("no_leak", after, before);
        }
#endif
        TestScore();
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    return 0;
}


