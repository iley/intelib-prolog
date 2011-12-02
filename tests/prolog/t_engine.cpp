//   InteLib                                    http://www.intelib.org
//   The file tests/prolog/t_data.cpp
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
#include "../tests.hpp"

#include "../../sexpress/iexcept.hpp"
#include "../../sexpress/sstring.hpp"
#include "../../prolog/prolog.hpp"
#include "../../sexpress/sexpress.hpp"


void poc()
{
#if INTELIB_DEBUG_COUNTERS == 1
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}

int main()
{
    try {
        poc();
        TestSection("PlgContext: frames");
        {
            //PlgDatabase db;
            PlgContext ctx;
            PlgContext::Frame *frame0 = ctx.CurrentFrame();

            TESTB("create frame #0", frame0 == 0);

            PlgContext::Frame *frame1;
            TESTB("create frame #1",
                    frame0 == ctx.CreateFrame() && (frame1 = ctx.CurrentFrame()) && frame0 != frame1);

            PlgVariableName X("X"), Y("Y"), Z("Z");
            PlgAtom f("f"), g("g"), h("h");

            ctx.Set(X, Y);
            TESTTR("assignment #1", ctx.Get(X), "Y");

            ctx.Set(Y, f);
            TESTTR("assignment #2", ctx.Get(Y), "f");

            ctx.Set(Z, f(Y));
            TESTTR("assignment #2.1", ctx.Get(Z), "f(Y)");

            PlgContext::Frame *frame2;
            TESTB("create frame #2", frame1 == ctx.CreateFrame() && (frame2 = ctx.CurrentFrame()) && frame1 != frame2);

            TESTB("unbound var", ctx.Get(X) == PlgUnbound);

            ctx.Set(Y, g);
            TESTTR("assignment #3", ctx.Get(Y), "g");

            ctx.Set(X, h);
            TESTTR("assignment #4", ctx.Get(X), "h");

            ctx.CreateFrame();
            ctx.Set(X, f);
            TESTTR("assignment #5", ctx.Get(X), "f");

            ctx.DropFrame();
            TESTB("drop frame #1", ctx.CurrentFrame() == frame2);
            TESTTR("drop frame #2", ctx.Get(X), "h");

            ctx.DropFrame(true);
            TESTB("drop frame #3", ctx.CurrentFrame() == frame1);
            TESTTR("drop frame #4", ctx.Get(X), "g");
            TESTTR("drop frame #5", ctx.Get(Y), "f");
            TESTTR("drop frame #6", ctx.Get(Z), "f(g)");
        }

        TestSection("PlgContext: evaluation");
        {
            PlgContext ctx;
            PlgVariableName X("X"), Y("Y"), Z("Z");
            PlgAtom f("f"), g("g"), h("h");

            ctx.CreateFrame();
            ctx.Set(X, h);
            ctx.Set(Y, Z);

            TESTTR("simple var", ctx.Evaluate(X), "h");
            TESTTR("simple term", ctx.Evaluate(f), "f");
            TESTTR("term with var", ctx.Evaluate(f(X)), "f(h)");
            TESTTR("nested term", ctx.Evaluate(f(g(X))), "f(g(h))");
            TESTTR("renamed var", ctx.Evaluate(f(Y)), "f(Z)");
            TESTTR("unbound var", ctx.Evaluate(f(Z)), "f(Z)");
            TESTTR("multiple vars", ctx.Evaluate(f(X,Y,Z)), "f(h, Z, Z)");
        }

        TestScore();
        poc();
    }
    catch(IntelibX &x) {
        printf("\nCaught IntelibX: %s\n", x.Description() );
        if(x.Parameter().GetPtr()) {
            printf("%s\n", x.Parameter()->TextRepresentation().c_str());
        }
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    poc();
    return 0;
}


