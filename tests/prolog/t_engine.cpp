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
        Hooks::EnableAll();
        PrintContextEnabled = true;

        TestSection("Context");
        {
            PlgContext ctx;

            PlgReference X = PlgVariableName("X");
            PlgReference Y = PlgVariableName("Y");
            PlgReference Z = PlgVariableName("Z");

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

            PlgReference X = PlgVariableName("X");
            PlgReference Y = PlgVariableName("Y");
            PlgReference Z = PlgVariableName("Z");
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
            PlgDatabase db;

            PlgReference (X) = PlgVariableName("X");
            PlgReference (Y) = PlgVariableName("Y");
            PlgAtom socrates("socrates"), plato("plato"), zeus("zeus"), mortal("mortal"), human("human"), man("man"), f("f");

            db.Add( man(plato)  );
            db.Add( man(socrates)  );
            db.Add( human(Y) <<= man(Y) );
            db.Add( mortal(X) <<= human(X) );
            db.Add( f(X) );

            Ok(db, f(X));
            Fail(db, f(X,X));
            Ok(db, man(socrates));
            Ok(db, human(socrates));
            Fail(db, human(zeus));
            Ok(db, mortal(socrates));
            Fail(db, mortal(zeus));
            Ok(db, mortal(X), X, (S| plato, socrates));
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

            Ok(db, a(b(a)));
            Fail(db, a(b(b)));
            Ok(db, a(c(b)));
            Fail(db, a(c(a)));
            Ok(db, a(X), X, (S| b(a), c(b)));
            Ok(db, always_true);
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

            Ok(db, X ^= f, X, (S| f ));
            Ok(db, g(X), X, (S| f ));
            Ok(db, h(X), X, (S| g ));
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

            Fail(db, f(alpha));
            Ok(db, f(beta));
            Ok(db, f(X), X, (S| beta));
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

            Ok(db, human(fry));
            Ok(db, human(leela));
            Fail(db, human(zoidberg));
            Fail(db, human(bender));
            Ok(db, human(X), X, (S| fry, leela));
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

            Ok(db, append((S|1,2,3), (S|4,5), (S|1,2,3,4,5)));
            Ok(db, append((S|1,2,3), (S|4,5), X), X, (S| (S|1,2,3,4,5) ));
            Ok(db, append((S|1,2,3), Nil, X), X, (S| (S|1,2,3) ));
            Ok(db, append(Nil, Nil, Nil));
            Ok(db, append(Nil, Nil, X), X, (S| Nil ));
            Ok(db, append((S|1,2,3), X, (S|1,2,3,4,5)), X, (S| (S|4,5)));

            Ok(db, member(1, (S|1,2,3)));
            Ok(db, member(2, (S|1,2,3)));
            Fail(db, member(1, (S|2,3,4)));
            Fail(db, member(1, Nil));
            Ok(db, member(X, (S|1,2,3)), X, (S|1,2,3));
            Ok(db, member(2, (S|1,X,3)), X, (S|2));
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

            Ok(db, a(X,Y), (S|X, Y), (S| (S|1, 10), (S|1, 20) ));
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

            Ok(db, gcd(12, 8, X), X, (S|4));
            Ok(db, gcd(12, 11, X), X, (S|1));
            Ok(db, gcd(32, 24, X), X, (S|8));
            Ok(db, gcd(24, 32, X), X, (S|8));
            Ok(db, gcd(2, 4, X), X, (S|2));
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
