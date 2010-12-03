//   InteLib                                    http://www.intelib.org
//   The file tools/vecread.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2010
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "../sexpress/svector.hpp"
#include "sreader.hpp"

static void do_process(int depth, SReference rest, SVectorRef &vec, bool rs)
{
    SExpressionCons *dp = rest.DynamicCastGetPtr<SExpressionCons>();
    if(!dp) {
        vec = new SExpressionVector(rs ? 0 : depth);
    } else {
        do_process(depth+1, dp->Cdr(), vec, rs);
        vec[depth] = dp->Car();
    }
}

static SReference process_fixed_vectors(const SReference &list, void*)
{
    SVectorRef vr;
    do_process(0, list, vr, false);
    return vr;
}

static SReference process_resizeable_vectors(const SReference &list, void*)
{
    SVectorRef vr;
    do_process(0, list, vr, true);
    return vr;
}

void add_vectors_to_reader(class IntelibReader &reader)
{
    reader.AddSequenceOpener("#(", process_fixed_vectors, 0, ")");
    reader.AddSequenceOpener("#~(", process_resizeable_vectors, 0, ")");
}
