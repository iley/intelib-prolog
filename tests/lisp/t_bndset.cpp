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
#include "genlisp/binds.hpp"


void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}


int main()
{
    poc();
    SLabel A("A"); SExpressionLabel *pa = A.GetPtr();
    SLabel B("B"); SExpressionLabel *pb = B.GetPtr();
    SLabel C("C"); SExpressionLabel *pc = C.GetPtr();
    SLabel D("D"); SExpressionLabel *pd = D.GetPtr();
    SLabel E("E"); SExpressionLabel *pe = E.GetPtr();
    SLabel F("F"); SExpressionLabel *pf = F.GetPtr();
    SExpressionLabel *pall[] = { pa, pb, pc, pd, pe, pf };
    try {
        TestSection("IntelibBindingsSet");
        TestSubsection("Creation");
        {
            IntelibBindingsSet t;
            TESTB("empty0", !t.GetBinding(pa));
            TESTB("empty25", !t.GetBinding(pb));
            *(t.AddBinding(pb)) = SReference(2525);
            TESTTR("root_only", *(t.GetBinding(pb)), "2525");
            *(t.AddBinding(pc)) = SReference(3636);
            TESTB("root_still_there", t.GetBinding(pb));
            TESTTR("root_still_has_value", *(t.GetBinding(pb)), "2525");
            TESTB("second_val_exists", t.GetBinding(pc));
            TESTTR("second_val", *(t.GetBinding(pc)), "3636");
            TESTB("root_still_there", t.GetBinding(pb));
        }
        TestSubsection("Allocation");
        {
            IntelibBindingsSet t;
            SReference* loc[4];
            for(int i = 0; i < 4; i++) {
                loc[i] = t.AddBinding(pall[i]);
                *loc[i] = (i+1) * 1000;
            }
            t.AddBinding(pf);
            SReference *l1 = t.GetBinding(pb);
            TESTB("still_there_after_resize", l1);
            TESTTR("still_value_after_resize", *l1, "2000");
            SReference *l0 = t.GetBinding(pa);
            TESTB("root_there_after_resize", l0);
            TESTTR("root_value_after_resize", *l0, "1000");
            //TESTB("resize_really_done", l1 != loc[1]);
        }
        TestSubsection("Iterator");
        {
            IntelibBindingsSet t;
            IntelibBindingsSet::Iterator iter0(t);
            SReference ref;
            const SExpressionLabel *key;
            TESTB("iterator_on_empty", !iter0.GetNext(key, ref));
            *(t.AddBinding(pa)) = SReference(2);
            *(t.AddBinding(pb)) = SReference(20);
            *(t.AddBinding(pc)) = SReference(200);
            IntelibBindingsSet::Iterator iter1(t);
            int sum2 = 0;
            while(iter1.GetNext(key, ref)) {
                if(ref.GetPtr()) sum2 += ref.GetInt();
            }
            TEST("iterator_values", sum2, 222);
        }
        TestSubsection("LotsOfBindings");
        {
            IntelibBindingsSet t;
            *(t.AddBinding(pa)) = SReference(1);
            *(t.AddBinding(pb)) = SReference(2);
            *(t.AddBinding(pc)) = SReference(3);
            *(t.AddBinding(pd)) = SReference(4);
            *(t.AddBinding(pe)) = SReference(5);
            *(t.AddBinding(pf)) = SReference(6);
            TESTB("value_there_1", t.GetBinding(pa));
            TESTB("value_there_2", t.GetBinding(pb));
            TESTB("value_there_3", t.GetBinding(pc));
            TESTB("value_there_4", t.GetBinding(pd));
            TESTB("value_there_5", t.GetBinding(pe));
            TESTB("value_there_6", t.GetBinding(pf));
        }
        TestScore();
    }
    catch(const IntelibX &ex) {
        printf("Caught IntelibX: %s\n%s\n",
            ex.Description(),
            ex.Parameter().GetPtr() ? 
                ex.Parameter()->TextRepresentation().c_str()
                : ""
        );
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    poc();
    return 0;
}


