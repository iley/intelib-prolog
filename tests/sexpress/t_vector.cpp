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
#include "sexpress/svector.hpp"


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
        TestSection("SVector");
        TestSubsection("Matrix");
        {
            SMatrixRef<5> matrix;
            matrix[1][2][1][0][1] = 123;
            matrix[2][1][0][1][4] = 777;

            TEST("matrix_1", matrix[1][2][1][0][1].GetInt(), 123);
            TEST("matrix_2", matrix[2][1][0][1][4].GetInt(), 777);
        }
        TestSubsection("Matrix_has_you");
        {
            SVector m;
            SVector m0;
            SVector m1;
            SVector m00;
            SVector m01;
            SVector m10;
            SVector m11;

            m10[1] = SReference(25);

            m0[0] = m00;
            m0[1] = m01;
            m1[0] = m10;
            m1[1] = m11;

            m[0]  = m0;
            m[1]  = m1;

            SMatrixRef<3> matr(m);

            TEST("matrix_has_you", matr[1][0][1].GetInt(), 25);
        }
        TestSubsection("resize");
        {
            SVector v;
            v[7] = 5;
            TEST("resize_1", v->Size(), 8);
            v[12] = 5;
            TEST("resize_2", v->Size(), 13);
            v[13] = 5;
            TEST("resize_3", v->Size(), 14);
        }
#if INTELIB_DEBUG_COUNTERS == 1
        TestSubsection("leaks");
        {
            int before = SExpression::object_counter;
            {
                SVector v;
                v[7] = 5;
                SVectorRef vr(v);
                SVectorRef vr2;
                vr2 = vr;
                SVectorRef vr3(vr2);
                for(int i=0; i<200; i++) vr3[vr3->Size()] = SReference(i);
            }
            TEST("no_leaks", SExpression::object_counter, before);
        }
#endif
        TestSubsection("TextRepresentation");
        {
            SVector v;
            for(int i=0; i<5; i++) v[i]=i;
            TEST("text_rep", v->TextRepresentation().c_str(),
                 "#<[0 1 2 3 4]>");

        }
        TestSubsection("Range Copying");
        {
            SVectorRef v;
            for(int i=0; i<15; i++) v[i]=i;

            SVectorRange r(v,5,3);
            TEST("range_copy", r.Copy()->TextRepresentation().c_str(),
                 "#<[5 6 7]>");
            TESTB("copy_keeps_positive_resizeability",
                 r.Copy()->IsResizeable());

            SVectorRef vn(5);
            for(int i=0; i<5; i++) vn[i]=i*100;
            SVectorRange rn(vn,1,2);
            TESTB("copy_keeps_negative_resizeability",
                 !rn.Copy()->IsResizeable());
            TESTB("copy_positive_resizeability",
                 rn.Copy(true)->IsResizeable());
            TESTB("copy_negative_resizeability",
                 !rn.Copy(false)->IsResizeable());

            SVectorRange r200(v,5,200);
            TEST("range_size_limited", r200.Copy()->Size(), 10);

        }
        TestSubsection("Range Erasing");
        {
            SVectorRef v;
            for(int i=0; i<15; i++) v[i]=i;
            SVectorRange r(v,3,10);
            r.Erase();
            TEST("range_erase", v->TextRepresentation().c_str(),
                 "#<[0 1 2 13 14]>");
            TEST("range_erase_size", v->Size(), 5);
            TEST("range_erase_range_len", r.Copy()->Size(), 0);

        }
        TestSubsection("Range Replacing");
        {
            SVectorRef v, w;
            for(int i=0; i<15; i++) v[i]=i;
            for(int i=0; i<5; i++) w[i]=i*100;

            SVectorRange r(v,3,10);
            r.Replace(w);
            TEST("range_replace_less", v->TextRepresentation().c_str(),
                 "#<[0 1 2 0 100 200 300 400 13 14]>");
            TEST("range_replace_less_size", v->Size(), 10);
            TEST("range_replace_less_range_len", r.Copy()->Size(), 5);
        }
        {
            SVectorRef v, w;
            for(int i=0; i<10; i++) v[i]=i;
            for(int i=0; i<5; i++) w[i]=i*100;

            SVectorRange r(w,1,2);
            r.Replace(v);
            TEST("range_replace_more", w->TextRepresentation().c_str(),
                 "#<[0 0 1 2 3 4 5 6 7 8 9 300 400]>");
            TEST("range_replace_more_size", w->Size(), 13);
            TEST("range_replace_more_range_len", r.Copy()->Size(), 10);
        }
        TestScore();
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    poc();
    return 0;
}


