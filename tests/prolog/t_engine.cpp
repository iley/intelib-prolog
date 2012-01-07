//   InteLib                                    http://www.intelib.org
//   The file tests/prolog/t_engine.cpp
//
//   Copyright (c) Ilya Strukov, 2011-2012
//
//
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
//
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.

#include <stdio.h>
#include "../../sexpress/iexcept.hpp"
#include "../../sexpress/sexpress.hpp"
#include "../../sexpress/sstring.hpp"
#include "../../prolog/prolog.hpp"
#include "../../prolog/utils.hpp"
#include "../tests.hpp"
#include "plgtest.hpp"

SListConstructor S;
SReference &Nil = *PTheEmptyList;

bool userPredicateCalled = false;

bool someUserPredicate(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
    userPredicateCalled = true;
    return true;
}

int main()
{
    try {
        //PlgGlobalHooks.Call = callHook;
        //PlgGlobalHooks.Unify = unifyHook;

        TestSection("Unification");
        {
            PlgContext ctx;

            PlgReference var0 = PlgVariableIndex(ctx.NextIndex());
            PlgReference var1 = PlgVariableIndex(ctx.NextIndex());
            PlgReference var2 = PlgVariableIndex(ctx.NextIndex());
            int pos = ctx.Top();

            PlgAtom f("f");

            TESTB("var0 <-> f (status)", var0.Unify(f, ctx));
            TESTTR("var0 <-> f (value)", var0.Evaluate(ctx), "f");
            TESTB("var0 <-> var0 where var0 = f", var0.Evaluate(ctx).Unify(var1, ctx));
            TESTTR("var0 <-> var0 where var0 = f (value)", var1.Evaluate(ctx), "f");
            ctx.ReturnTo(pos);

            ctx.Set(var1, f);
            TESTB("f(var0) <-> f(var1) (status)", f(var0).Unify(f(var1), ctx));
            TESTB("f(var0) <-> f(var) (value)", var0.Evaluate(ctx) == f);
            ctx.ReturnTo(pos);

            ctx.Set(var1, f);
            TESTB("f(var0, var0) <-> f(var1, var2)", f(var1, var2).Evaluate(ctx).Unify(f(var0, var0), ctx));
            TESTB("f(var0, var0) <-> f(var1, var2) (value 1)", var0.Evaluate(ctx) == f);
            TESTB("f(var0, var0) <-> f(var1, var2) (value 2)", var2.Evaluate(ctx) == f);
            ctx.ReturnTo(pos);

            SListConstructor S;
            PlgReference list1 = (S|1, 2), list2 = (S|1, 2), list3 = (S|var0, 2);
            TESTB("(1 2) <-> (1 2)", list1.Unify(list2, ctx));
            TESTB("(1 2) <-> (X 2)", list1.Unify(list3, ctx));
            TESTTR("(1 2) <-> (X 2) (value)", var0.Evaluate(ctx), "1");
            ctx.ReturnTo(pos);
        }
        TestScore();

        TestSection("Solving 1");
        {
            PlgDatabase db;

            PlgReference (X) = PlgVariableName("X");
            PlgReference (Y) = PlgVariableName("Y");
            PlgAtom socrates("socrates"), plato("plato"), zeus("zeus"), mortal("mortal"), human("human"), man("man"), f("f");

            db.Add( man(plato)  );
            db.Add( man(socrates)  );
            db.Add( human(Y) <<= man(Y) );
            db.Add( mortal(X) <<= human(X) );
            db.Add( f(X) );

            ok(db, f(X));
            fail(db, f(X,X));
            ok(db, man(socrates));
            ok(db, human(socrates));
            fail(db, human(zeus));
            ok(db, mortal(socrates));
            fail(db, mortal(zeus));
            ok(db, mortal(X), X, (S| plato, socrates));
        }
        TestScore();

        TestSection("Solving 2");
        {
            PlgAtom a("a"), b("b"), c("c"), d("d"), f("f"), always_true("always_true");
            PlgVariableName X("X");
            PlgDatabase db;

            db.Add( a(b(X)) <<= d(X) );
            db.Add( a(c(X)) <<= f(X) );
            db.Add( d(a) );
            db.Add( f(b) );
            db.Add( always_true );

            ok(db, a(b(a)));
            fail(db, a(b(b)));
            ok(db, a(c(b)));
            fail(db, a(c(a)));
            ok(db, a(X), X, (S| b(a), c(b)));
            ok(db, always_true);
        }
        TestScore();

        TestSection("User predicates");
        {
            PlgAtom f("f"), g("g"), h("h");
            PlgVariableName X("X"), Y("Y");
            PlgDatabase db;

            db.Add( g(X) <<= (X ^= f) );
            db.Add( h(X) <<= h(X, g) );
            db.Add( h(X, Y) <<= (X ^= Y) );

            f->SetPredicate(1, someUserPredicate);
            PlgContinuation cont = db.Query(f(X));
            TESTB("before calling f", !userPredicateCalled);
            TESTB("calling f", cont->Next());
            TESTB("after calling f", userPredicateCalled);
            TESTB("calling f for a second time", !cont->Next());

            ok(db, X ^= f, X, (S| f ));
            ok(db, g(X), X, (S| f ));
            ok(db, h(X), X, (S| g ));
        }
        TestScore();

        TestSection("Conjunction");
        {
            PlgAtom f("f"), g("g"), h("h");
            PlgAtom alpha("alpha"), beta("beta");
            PlgVariableName X("X");
            PlgDatabase db;

            db.Add( f(X) <<= g(X) & h(X) );
            db.Add( g(alpha) );
            db.Add( g(beta) );
            db.Add( h(beta) );

            fail(db, f(alpha));
            ok(db, f(beta));
            ok(db, f(X), X, (S| beta));
        }
        TestScore();

        TestSection("Disjunction");
        {
            PlgAtom human("human"), man("man"), woman("woman"), robot("robot"), alien("alien");
            PlgAtom fry("fry"), leela("leela"), bender("bender"), zoidberg("zoidberg");
            PlgVariableName X("X");
            PlgDatabase db;

            db.Add( man(fry) );
            db.Add( woman(leela) );
            db.Add( robot(bender) );
            db.Add( alien(zoidberg) );
            db.Add( human(X) <<= man(X) | woman(X) );

            ok(db, human(fry));
            ok(db, human(leela));
            fail(db, human(zoidberg));
            fail(db, human(bender));
            ok(db, human(X), X, (S| fry, leela));
        }
        TestScore();

        TestSection("Lists");
        {
            PlgAtom append("append"), member("member");
            PlgVariableName X("X"), H("H"), T("T"), R("R"), L("L");
            PlgDatabase db;
            SListConstructor S;

            db.Add( append(*PTheEmptyList, X, X) );
            db.Add( append(H^T, L, H^R) <<= append(T, L, R) );
            db.Add( member(X, H^T) <<= (X ^= H) | member(X, T) );

            ok(db, append((S|1,2,3), (S|4,5), (S|1,2,3,4,5)));
            ok(db, append((S|1,2,3), (S|4,5), X), X, (S| (S|1,2,3,4,5) ));
            ok(db, append((S|1,2,3), Nil, X), X, (S| (S|1,2,3) ));
            ok(db, append(Nil, Nil, Nil));
            ok(db, append(Nil, Nil, X), X, (S| Nil ));
            ok(db, append((S|1,2,3), X, (S|1,2,3,4,5)), X, (S| (S|4,5)));

            ok(db, member(1, (S|1,2,3)));
            ok(db, member(2, (S|1,2,3)));
            fail(db, member(1, (S|2,3,4)));
            fail(db, member(1, Nil));
            ok(db, member(X, (S|1,2,3)), X, (S|1,2,3));
            ok(db, member(2, (S|1,X,3)), X, (S|2));
        }
        TestScore();

        TestSection("Cut");
        {
            using PlgStdLib::cut;

            PlgAtom a("a"), b("b"), c("c");
            PlgVariableName X("X"), Y("Y");
            PlgDatabase db;

            db.Add( a(X, Y) <<= b(X) & cut & c(Y) );
            db.Add( b(1) );
            db.Add( b(2) );
            db.Add( b(3) );
            db.Add( c(10) );
            db.Add( c(20) );

            ok(db, a(X,Y), (S|X, Y), (S| (S|1, 10), (S|1, 20) ));
        }
        TestScore();

        TestSection("Numbers");
        {
            using PlgStdLib::is;
            using PlgStdLib::cut;

            //TODO: more tests
            PlgAtom gcd("gcd");
            PlgVariableName X("X"), Y("Y"), Z("Z"), D("D");
            PlgDatabase db;

            db.Add( gcd(X, 0, X) <<= cut );
            db.Add( gcd(0, X, X) <<= cut );
            db.Add( gcd(X, Y, D) <<= (X > Y) & cut & is(Z, X % Y) & gcd(Y, Z, D) );
            db.Add( gcd(X, Y, D) <<= is(Z, Y % X) & gcd(X, Z, D) );

            ok(db, gcd(12, 8, X), X, (S|4));
            ok(db, gcd(12, 11, X), X, (S|1));
            ok(db, gcd(32, 24, X), X, (S|8));
            ok(db, gcd(24, 32, X), X, (S|8));
            ok(db, gcd(2, 4, X), X, (S|2));
        }
        TestScore();
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

    return 0;
}
