//   InteLib                                    http://www.intelib.org
//   The file tests/refal/t_markup.cpp
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

#include "refal/refal.hpp"



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

int main()
{
    poc();
#ifdef INTELIB_DEBUG_COUNTERS
    int before = SExpression::object_counter;
#endif
    try {
        TestSection("RefalMarkup");
#if 0
        TestSubsection("StructureParentheses");
        {
            RfNodeRef ls;
            ls,RfExpressionNode::left_par;
            RfNodeRef beg = ls;
            ls,SReference(1);
            ls,RfExpressionNode::right_par;
            RfNodeRef end = ls->Next()->Next();
            TESTTR("before_pairing", beg, "(# 1 #)");
            beg->Pair(end);
            TESTTR("after_pairing", ls, "( 1 )");
        }
        TestSubsection("RefalCalls");
        {
            RfNodeRef ls;
            ls,RfExpressionNode::left_call;
            RfNodeRef beg = ls;
            ls,SReference(1);
            ls,RfExpressionNode::right_call;
            RfNodeRef end = ls->Next()->Next();
            TESTTR("before_pairing", beg, "<# 1 #>");
            beg->Pair(end);
            TESTTR("after_pairing", ls, "< 1 >");
        }
#endif
        TestSubsection("RefalMarkupIterator");
        {
            RfFormConstructor R;
            RfReference view((R|"a",(R|"bcd"),"ef"));

            RfListItem *i = view->GetFirst();

            TESTTR("markup_iter_init", i,
                   "a(bcd)ef");
            i = i->NextTerm();
            TESTB("markup_iter_forward_", i != 0);
            TESTTR("markup_iter_forward", i,
                   "(bcd)ef");
            i = i->NextTerm();
            TESTTR("markup_iter_jump_over_markup", i,
                   "ef");
            i = i->NextTerm();
            TESTTR("markup_iter_prefix_forward", i,
                   "f");

            i = i->PrevTerm();
            TESTTR("markup_iter_backward", i,
                   "ef");

            i = i->PrevTerm();
            TESTTR("markup_iter_jump_back", i,
                   "(bcd)ef");

            i = i->PrevTerm();
            TESTTR("markup_prefix_backward", i,
                   "a(bcd)ef");
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


