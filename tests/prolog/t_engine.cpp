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
#include "../../prolog/plutils.hpp"
#include "../tests.hpp"
#include "plgtest.hpp"

SListConstructor S;
SReference &Nil = *(GetEmptyList());

bool userPredicateCalled = false;

bool someUserPredicate(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
    userPredicateCalled = true;
    return true;
}

int main()
{
    using PlgStdLib::truth;

    try {
        //Hooks::EnableAll();
        //PrintContextEnabled = true;

        TestSection("Context");
        {
            PlgContext ctx;

            PlgReference X = PlgVariable("X");
            PlgReference Y = PlgVariable("Y");
            PlgReference Z = PlgVariable("Z");

            PlgAtom f("f"), g("g");

            int frame0 = ctx.NextFrame();
            ctx.Set(X, f);
            printContext(ctx);
            TESTTR("X = f", X.Evaluate(ctx), "f");
            ctx.Set(Z, g);
            printContext(ctx);
            TESTTR("X = f", X.Evaluate(ctx), "f");
            TESTTR("Z = g", Z.Evaluate(ctx), "g");
            TESTTR("Y = Y", Y.Evaluate(ctx), "Y");

            int frame1 = ctx.NextFrame();
            ctx.Set(Y, X);
            TESTTR("Y = f", Y.Evaluate(ctx), "f");

            ctx.ReturnTo(frame1);
            TESTTR("X = f", X.Evaluate(ctx), "f");
            TESTTR("Y = Y", Y.Evaluate(ctx), "Y");

            ctx.ReturnTo(frame0);
            TESTTR("X = X", X.Evaluate(ctx), "X");
            TESTTR("Y = Y", Y.Evaluate(ctx), "Y");
        }

        TestSection("Unification");
        {
            PlgContext ctx;

            PlgReference X = PlgVariable("X");
            PlgReference Y = PlgVariable("Y");
            PlgReference Z = PlgVariable("Z");
            int pos = ctx.Top();

            PlgAtom f("f");

            ctx.NextFrame();
            TESTB("X <-> f (status)", X.Unify(f, ctx));
            printContext(ctx);
            TESTTR("X <-> f (value)", X.Evaluate(ctx), "f");
            TESTB("X <-> X where X = f", X.Evaluate(ctx).Unify(Y, ctx));
            TESTTR("X <-> X where X = f (value)", Y.Evaluate(ctx), "f");
            ctx.ReturnTo(pos);

            ctx.NextFrame();
            ctx.Set(Y, f);
            TESTB("f(X) <-> f(Y) (status)", f(X).Unify(f(Y), ctx));
            TESTB("f(X) <-> f(Y) (value)", X.Evaluate(ctx) == f);
            ctx.ReturnTo(pos);

            ctx.NextFrame();
            ctx.Set(Y, f);
            TESTB("f(X, X) <-> f(Y, Z)", f(Y, Z).Evaluate(ctx).Unify(f(X, X), ctx));
            TESTB("f(X, X) <-> f(Y, Z) (value 1)", X.Evaluate(ctx) == f);
            TESTB("f(X, X) <-> f(Y, Z) (value 2)", Z.Evaluate(ctx) == f);
            ctx.ReturnTo(pos);

            ctx.NextFrame();
            SListConstructor S;
            PlgReference list1 = (S|1, 2), list2 = (S|1, 2), list3 = (S|X, 2);
            TESTB("(1 2) <-> (1 2)", list1.Unify(list2, ctx));
            printContext(ctx);
            TESTB("(1 2) <-> (X 2)", list1.Unify(list3, ctx));
            printContext(ctx);
            TESTTR("(1 2) <-> (X 2) (value)", X.Evaluate(ctx), "1");
            printContext(ctx);
            ctx.ReturnTo(pos);
        }
        TestScore();

        TestSection("Solving 1");
        {
            PlgReference (X) = PlgVariable("X");
            PlgReference (Y) = PlgVariable("Y");
            PlgAtom socrates("socrates"), plato("plato"), zeus("zeus"), mortal("mortal"), human("human"), man("man"), f("f");

            man(plato) <<= truth;
            man(socrates) <<= truth;
            human(Y) <<= man(Y);
            mortal(X) <<= human(X);
            f(X)<<= truth;

            Ok(f(X));
            Fail(f(X,X));
            Ok(man(socrates));
            Ok(human(socrates));
            Fail(human(zeus));
            Ok(mortal(socrates));
            Fail(mortal(zeus));
            Ok(mortal(X), X, (S| plato, socrates));
        }
        TestScore();

        TestSection("Solving 2");
        {
            PlgAtom a("a"), b("b"), c("c"), d("d"), f("f"), g("g"), always_true("always_true");
            PlgVariable X("X");

            using PlgStdLib::_;

            Assert( a(b(X)) << d(X) );
            Assert( a(c(X)) << f(X) );
            Assert( d(a) );
            Assert( f(b) );
            Assert( g(_) );
            Assert( always_true );

            Ok(a(b(a)));
            Fail(a(b(b)));
            Ok(a(c(b)));
            Fail(a(c(a)));
            Ok(a(X), X, (S| b(a), c(b)));
            Ok(always_true);
            Ok(g(a));
            Ok(g(b));
        }
        TestScore();

        TestSection("User predicates");
        {
            PlgAtom u("u"), g("g"), h("h");
            PlgVariable X("X"), Y("Y");

            Assert( g(X) << (X ^= u) );
            Assert( h(X) << h(X, g) );
            Assert( h(X, Y) << (X ^= Y) );

            u.SetPredicate(someUserPredicate, 1);
            PlgContinuation cont = u(X).Query();
            TESTB("before calling u", !userPredicateCalled);
            TESTB("calling u", cont->Next());
            TESTB("after calling u", userPredicateCalled);
            TESTB("calling u for a second time", !cont->Next());

            Ok(X ^= u, X, (S| u ));
            Ok(g(X), X, (S| u ));
            Ok(h(X), X, (S| g ));
        }
        TestScore();

        TestSection("Conjunction");
        {
            PlgAtom f("f"), g("g"), h("h");
            PlgAtom alpha("alpha"), beta("beta");
            PlgVariable X("X");

            f(X) <<= g(X) & h(X);
            Assert( g(alpha) );
            Assert( g(beta) );
            Assert( h(beta) );

            Fail(f(alpha));
            Ok(f(beta));
            Ok(f(X), X, (S| beta));
        }
        TestScore();

        TestSection("Disjunction");
        {
            PlgAtom human("human"), man("man"), woman("woman"), robot("robot"), alien("alien");
            PlgAtom fry("fry"), leela("leela"), bender("bender"), zoidberg("zoidberg");
            PlgVariable X("X");

            Assert( man(fry) );
            Assert( woman(leela) );
            Assert( robot(bender) );
            Assert( alien(zoidberg) );
            human(X) <<= man(X) | woman(X);

            Ok(human(fry));
            Ok(human(leela));
            Fail(human(zoidberg));
            Fail(human(bender));
            Ok(human(X), X, (S| fry, leela));
        }
        TestScore();

        TestSection("Lists");
        {
            PlgAtom append("append"), member("member");
            PlgVariable X("X"), H("H"), T("T"), R("R"), L("L");
            SListConstructor S;

            append(Nil, X, X) <<= truth;
            append(H^T, L, H^R) <<= append(T, L, R);
            member(X, H^T) <<= (X ^= H) | member(X, T);

            Ok(append((S|1,2,3), (S|4,5), (S|1,2,3,4,5)));
            Ok(append((S|1,2,3), (S|4,5), X), X, (S| (S|1,2,3,4,5) ));
            Ok(append((S|1,2,3), Nil, X), X, (S| (S|1,2,3) ));
            Ok(append(Nil, Nil, Nil));
            Ok(append(Nil, Nil, X), X, (S| Nil ));
            Ok(append((S|1,2,3), X, (S|1,2,3,4,5)), X, (S| (S|4,5)));

            Ok(member(1, (S|1,2,3)));
            Ok(member(2, (S|1,2,3)));
            Fail(member(1, (S|2,3,4)));
            Fail(member(1, Nil));
            Ok(member(X, (S|1,2,3)), X, (S|1,2,3));
            Ok(member(2, (S|1,X,3)), X, (S|2));
        }
        TestScore();

        TestSection("Cut");
        {
            using PlgStdLib::cut;

            PlgAtom a("a"), b("b"), c("c");
            PlgVariable X("X"), Y("Y"), Z("Z");

            a(X, Y) <<= b(X) & cut & c(Y);
            Assert( b(1) );
            Assert( b(2) );
            Assert( b(3) );
            Assert( c(10) );
            Assert( c(20) );

            Ok(a(X,Y), (S|X, Y), (S| (S|1, 10), (S|1, 20) ));

            PlgAtom f("f"), g("g");
            
            Assert( f(1) );
            Assert( f(2) );
            Assert( g(1) << cut );
            Assert( g(2) );

            Ok(f(X) & g(Y), (S|X,Y), (S| (S|1, 1), (S|2, 1) ));
            Ok(b(X) & c(Y) & cut & f(Z), (S|X,Y,Z), (S| (S|1, 10, 1), (S|1, 10, 2)));
        }
        TestScore();

        TestSection("Numbers");
        {
            using PlgStdLib::is;
            using PlgStdLib::cut;

            //TODO: more tests
            PlgAtom gcd("gcd");
            PlgVariable X("X"), Y("Y"), Z("Z"), D("D");

            gcd(X, 0, X) <<= cut;
            gcd(0, X, X) <<= cut;
            gcd(X, Y, D) <<= (X > Y) & cut & is(Z, X % Y) & gcd(Y, Z, D);
            gcd(X, Y, D) <<= is(Z, Y % X) & gcd(X, Z, D);

            Ok(gcd(12, 8, X), X, (S|4));
            Ok(gcd(12, 11, X), X, (S|1));
            Ok(gcd(32, 24, X), X, (S|8));
            Ok(gcd(24, 32, X), X, (S|8));
            Ok(gcd(2, 4, X), X, (S|2));
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
