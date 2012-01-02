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

#include "../../prolog/utils.hpp"

void poc()
{
#if INTELIB_DEBUG_COUNTERS == 1
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}

void printContext(const PlgContext &context) {
    return;
    printf("--- context dump start ---\n");

    printf("%s", DumpContext(context).c_str());

    printf("---  context dump end  ---\n");
}

bool userPredicateCalled = false;

bool someUserPredicate(const SReference &args, PlgExpressionContinuation &cont) {
    userPredicateCalled = true;
    return true;
}

int main()
{
    try {
        poc();
        //TODO: replace old tests
        /*
        TestSection("Context frames");
        {
            //PlgDatabase db;
            PlgContext ctx;
            PlgContext::Frame *frame0 = ctx.Top();

            TESTB("create frame #0", !!frame0);

            PlgContext::Frame *frame1;
            TESTB("create frame #1",
                    frame0 == ctx.CreateFrame() && (frame1 = ctx.Top()) && frame0 != frame1);

            PlgVariableName X("X"), Y("Y"), Z("Z");
            PlgAtom f("f"), g("g"), h("h");

            ctx.Set(X, Y);
            TESTTR("assignment #1", ctx.Get(X), "Y");

            ctx.Set(Y, f);
            TESTTR("assignment #2", ctx.Get(Y), "f");

            ctx.Set(Z, f(Y));
            TESTTR("assignment #2.1", ctx.Get(Z), "f(Y)");

            PlgContext::Frame *frame2;
            TESTB("create frame #2", frame1 == ctx.CreateFrame() && (frame2 = ctx.Top()) && frame1 != frame2);

            TESTB("unbound var", ctx.Get(X) == PlgUnbound);

            ctx.Set(Y, g);
            TESTTR("assignment #3", ctx.Get(Y), "g");

            ctx.Set(X, h);
            TESTTR("assignment #4", ctx.Get(X), "h");

            ctx.CreateFrame();
            ctx.Set(X, f);
            TESTTR("assignment #5", ctx.Get(X), "f");

            ctx.DropFrame();
            TESTB("drop frame #1", ctx.Top() == frame2);
            TESTTR("drop frame #2", ctx.Get(X), "h");

            ctx.DropFrame(true);
            TESTB("drop frame #3", ctx.Top() == frame1);
            TESTTR("drop frame #4", ctx.Get(X), "g");
            TESTTR("drop frame #5", ctx.Get(Y), "f");
            TESTTR("drop frame #6", ctx.Get(Z), "f(g)");
        }
        */

        //TODO: replace old tests
        /*
        TestSection("Context evaluation");
        {
            PlgContext ctx;
            PlgVariableName X("X"), Y("Y"), Z("Z");
            PlgAtom f("f"), g("g"), h("h");

            //ctx.CreateFrame();
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
        */

        TestSection("Unification");
        {
            PlgContext ctx;
            
            PlgReference var0 = PlgVariableIndex(ctx.NextIndex());
            PlgReference var1 = PlgVariableIndex(ctx.NextIndex());
            PlgReference var2 = PlgVariableIndex(ctx.NextIndex());
            int pos = ctx.Top();

            PlgAtom f("f");

            TESTB("var0 <-> f (status)", var0.Unify(f, ctx));
            TESTTR("var0 <-> f (value)", ctx.Evaluate(var0), "f");
            TESTB("var0 <-> var0 where var0 = f (status)", ctx.Evaluate(var0).Unify(var1, ctx));
            TESTTR("var0 <-> var0 where var0 = f (value)", ctx.Evaluate(var1), "f");
            ctx.ReturnTo(pos);

            ctx.Set(var1, f);
            TESTB("f(var0) <-> f(var1) (status)", f(var0).Unify(f(var1), ctx));
            TESTB("f(var0) <-> f(var) (value)", ctx.Evaluate(var0) == f);
            ctx.ReturnTo(pos);

            ctx.Set(var1, f);
            TESTB("f(var0, var0) <-> f(var1, var2) (status)", ctx.Evaluate(f(var1, var2)).Unify(f(var0, var0), ctx));
            TESTB("f(var0, var0) <-> f(var1, var2) (value 1)", ctx.Evaluate(var0) == f);
            TESTB("f(var0, var0) <-> f(var1, var2) (value 2)", ctx.Evaluate(var2) == f);
            ctx.ReturnTo(pos);
        }
        TestScore();

        TestSection("Solving 1");
        {
            PlgDatabase db;
            //TODO: facts

            PlgReference (X) = PlgVariableName("X");
            PlgReference (Y) = PlgVariableName("Y");
            PlgAtom socrates("socrates"), plato("plato"), zeus("zeus"), mortal("mortal"), human("human"), man("man"), f("f");

            db.Add( man(plato) <<= PlgTrue );
            db.Add( man(socrates) <<= PlgTrue );
            db.Add( human(Y) <<= man(Y) );
            db.Add( mortal(X) <<= human(X) );
            db.Add( f(X) <<= PlgTrue );

            PlgContinuation cont = db.Query( f(X) );
            TESTB("f(X)", cont->Next());
            printContext(cont->Context());

            cont = db.Query( f(X, X) );
            TESTB("f(X,X)", !cont->Next());
            printContext(cont->Context());

            cont = db.Query( man(socrates) );
            TESTB("man(socrates)", cont->Next());
            printContext(cont->Context());

            cont = db.Query( human(socrates) );
            TESTB("human(socrates)", cont->Next());
            printContext(cont->Context());

            cont = db.Query( human(zeus) );
            TESTB("human(zeus)", !cont->Next());
            printContext(cont->Context());

            cont = db.Query( mortal(socrates) );
            TESTB("mortal(socrates)", cont->Next());
            printContext(cont->Context());

            cont = db.Query( mortal(zeus) );
            TESTB("mortal(zeus)", !cont->Next());
            printContext(cont->Context());

            cont = db.Query( mortal(X) );
            TESTB("mortal(X) #1", cont->Next());
            printContext(cont->Context());
            TESTTR("mortal(X) where X = plato", cont->GetValue(X), "plato");

            TESTB("mortal(X) #2", cont->Next());
            printContext(cont->Context());
            TESTTR("mortal(X) where X = socrates", cont->GetValue(X), "socrates");

            TESTB("mortal(X) end", !cont->Next());
            printContext(cont->Context());
        }
        TestScore();

        TestSection("Solving 2");
        {
            PlgAtom a("a"), b("b"), c("c"), d("d"), f("f");
            PlgVariableName X("X");
            PlgDatabase db;

            db.Add( a(b(X)) <<= d(X) );
            db.Add( a(c(X)) <<= f(X) );
            db.Add( d(a) <<= PlgTrue );
            db.Add( f(b) <<= PlgTrue );

            PlgContinuation cont = db.Query(a(b(a)));
            TESTB("a(b(a))", cont->Next());
            printContext(cont->Context());

            cont = db.Query(a(b(b)));
            TESTB("a(b(b))", !cont->Next());
            printContext(cont->Context());

            cont = db.Query(a(c(b)));
            TESTB("a(c(b))", cont->Next());
            printContext(cont->Context());

            cont = db.Query(a(c(a)));
            TESTB("a(c(a))", !cont->Next());
            printContext(cont->Context());

            cont = db.Query(a(X));
            TESTB("a(X) #1", cont->Next());
            TESTTR("a(X) where X = b(a)", cont->GetValue(X), "b(a)");
            printContext(cont->Context());

            TESTB("a(X) #2", cont->Next());
            TESTTR("a(X) where X = c(b)", cont->GetValue(X), "c(b)");
            printContext(cont->Context());

            TESTB("a(X) #3", !cont->Next());
            printContext(cont->Context());
        }
        TestScore();

        TestSection("User predicates");
        {
            PlgAtom f("f"), g("g"), h("h");
            PlgVariableName X("X"), Y("Y");
            PlgDatabase db;

            f->SetPredicate(1, someUserPredicate);
            PlgContinuation cont = db.Query(f(X)); //FIXME: cast atom to 0-arity term
            TESTB("before calling f", !userPredicateCalled);
            TESTB("calling f", cont->Next());
            TESTB("after calling f", userPredicateCalled);
            TESTB("calling f for a second time", !cont->Next());

            cont = db.Query(X ^= f);
            TESTB("evaluate X = f", cont->Next());
            TESTTR("get X value in X = f", cont->GetValue(X), "f");
            TESTB("evaluate X = f for a second time", !cont->Next());

            db.Add( g(X) <<= (X ^= f) );
            cont = db.Query(g(X));
            printContext(cont->Context());
            TESTB("evaluate g(X)", cont->Next());
            printContext(cont->Context());
            TESTTR("get X in g(X)", cont->GetValue(X), "f");
            TESTB("evaluate g(X) for a second time", !cont->Next());

            db.Add( h(X) <<= h(X, g) );
            db.Add( h(X, Y) <<= (X ^= Y) );
            cont = db.Query(h(X));
            printContext(cont->Context());
            TESTB("evaluate h(X)", cont->Next());
            printContext(cont->Context());
            TESTTR("get X in h(X)", cont->GetValue(X), "g");
            TESTB("evaluate h(X) for a second time", !cont->Next());
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
