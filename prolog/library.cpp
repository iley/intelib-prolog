#include "../sexpress/shashtbl.hpp"
#include "library.hpp"
#include "utils.hpp"

bool PlgDefaultPredicate(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
{
    PlgClauseChoicePoint cp(PlgTerm(functor, args), cont);
    cont.PushChoicePoint(cp);
    return false; //to force backtracking
}

namespace PlgStdLib
{
    static SListConstructor S;

    // Conjunction

    bool PredicateConjunction(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgReference left = args.Car();
        PlgReference right = args.Cdr().Car();
        cont.PushQuery(right);
        cont.PushQuery(left);
        return true;
    }

    PlgAtom conjunction(",", 2, PredicateConjunction, true);

    // Disjunction

    bool PredicateDisjunction(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgDisjChoicePoint cp(args, cont);
        cont.PushChoicePoint(cp);
        return false; //to force backtracking
    }

    PlgAtom disjunction(";", 2, PredicateDisjunction, true);

    // "=" predicate

    bool PredicateUnifies(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgReference left = args.Car();
        PlgReference right = args.Cdr().Car();

        return left.Unify(right, cont.Context());
    }

    PlgAtom unifies("=", 2, PredicateUnifies, true);

    // Truth value
    bool PredicateTrue(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        return true;
    }

    PlgAtom truth("true", 0, PredicateTrue, false);

    // Truth value
    bool PredicateFail(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        return false;
    }

    PlgAtom fail("fail", 0, PredicateFail, false);

    // "/=" predicate

    bool PredicateNotUnifies(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        int pos = cont.Context().Top();
        PlgReference left = args.Car(),
                     right = args.Cdr().Car();
        bool result = left.Unify(right, cont.Context());
        cont.Context().ReturnTo(pos);
        return !result;
    }

    PlgAtom not_unifies("/=", 2, PredicateNotUnifies, true);

    // Cut

    bool PredicateCut(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        cont.ResetChoicePoints();
        return true;
    }

    PlgAtom cut("!", 0, PredicateCut, false);

    // Assert

    bool PredicateAssertA(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgReference arg = args.Car();
        cont.Database().AddA(arg);
        return true;
    }

    PlgAtom assert("assert", 1, PredicateAssertA, false);
    PlgAtom asserta("asserta", 1, PredicateAssertA, false);

    bool PredicateAssertZ(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgReference arg = args.Car();
        cont.Database().Add(arg);
        return true;
    }

    PlgAtom assertz("assertz", 1, PredicateAssertZ, false);


    // Integer arithmetic

    PlgAtom minus("-", 2);
    PlgAtom plus("+", 2);
    PlgAtom multiply("*", 2);
    PlgAtom divide("/", 2);
    PlgAtom reminder("%", 2);

    static int IntEval(const PlgReference &expr)
    {
        INTELIB_ASSERT(expr.GetPtr(), IntelibX_unexpected_unbound_value());

        if (expr->TermType() == SExpressionInt::TypeId) {
            return expr.GetInt();
        } else if (expr->TermType() == PlgExpressionTerm::TypeId) {
            PlgTerm term = expr;
            PlgAtom oper = term->Functor();
            int left = IntEval(term->Args().Car()),
                right = IntEval(term->Args().Cdr().Car()),
                result;

            if (oper == minus)
                result = left - right;
            else if (oper == plus)
                result = left + right;
            else if (oper == multiply)
                result = left * right;
            else if (oper == divide)
                result = left / right;
            else if (oper == reminder)
                result = left % right;
            else
                throw IntelibX_not_implemented();

            return result;
        } else {
            throw IntelibX_not_implemented();
        }
    }

    bool PredicateIs(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgReference left = args.Car(),
                     right = args.Cdr().Car();

        return left.Unify(SReference(IntEval(right.Evaluate(cont.Context()))), cont.Context());
    }

    PlgAtom is("is", 2, PredicateIs, true);

    bool PredicateIntEqual(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgReference left = args.Car(),
                     right = args.Cdr().Car();
        return IntEval(left) == IntEval(right);
    }

    PlgAtom int_equal("=:=", PredicateIntEqual, true);

    bool PredicateIntNotEqual(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgReference left = args.Car(),
                     right = args.Cdr().Car();
        return IntEval(left) != IntEval(right);
    }

    PlgAtom int_not_equal("=\\=", PredicateIntNotEqual, true);

    bool PredicateIntLess(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgReference left = args.Car(),
                     right = args.Cdr().Car();
        return IntEval(left) < IntEval(right);
    }

    PlgAtom int_less("<", 2, PredicateIntLess, true);

    bool PredicateIntLessOrEqual(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgReference left = args.Car(),
                     right = args.Cdr().Car();
        return IntEval(left) <= IntEval(right);
    }

    PlgAtom int_less_or_equal("<=", 2, PredicateIntLessOrEqual, true);

    bool PredicateIntGreater(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgReference left = args.Car(),
                     right = args.Cdr().Car();
        return IntEval(left) > IntEval(right);
    }

    PlgAtom int_greater(">", 2, PredicateIntGreater, true);

    bool PredicateIntGreaterOrEqual(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgReference left = args.Car(),
                     right = args.Cdr().Car();
        return IntEval(left) >= IntEval(right);
    }

    PlgAtom int_greater_or_equal(">=", 2, PredicateIntGreaterOrEqual);

    bool LibraryPredicate(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont);

    // Database with standard predicates written in prolog

    void InitDb(PlgDatabase &db)
    {
        PlgVariableName X("X"), H("H"), T("T"), L("L"), R("R"), N("N"), N1("N1");
        SReference &Nil = *PTheEmptyList;

        db.Add( nope(X) <<= (X & cut & fail) | truth ); // not(X) :- X, !, fail; true

        db.Add( append(Nil, X, X) );
        db.Add( append(H^T, L, H^R) <<= append(T, L, R) );

        db.Add( member(X, H^T) <<= ((X ^= H) & cut) | member(X, T) );

        db.Add( length(Nil, 0) <<= cut );
        db.Add( length(H^T, N) <<= length(T,N1) & N.is(N1 + SReference(1)) );

        PlgAtom index("index", 4, LibraryPredicate, false);
        PlgVariableName StartIndex("StartIndex");

        // index/4 is an auxilary predicate to implement nth and nth0
        db.Add( index(StartIndex, N, L, R) <<= (N < StartIndex) & cut & fail );
        db.Add( index(StartIndex, StartIndex, H^T, H) <<= cut );
        db.Add( index(StartIndex, N, H^T, X) <<= N1.is(N - SReference(1)) & index(StartIndex, N1, T, X) );

        db.Add( nth(N, L, X) <<= index(1, N, L, X) );
        db.Add( nth0(N, L, X) <<= index(0, N, L, X) );

        db.Add( permutation(Nil, Nil) );
        db.Add( permutation(H^T, R) <<= permutation(T,L) & select(H,R,L) );

        db.Add( select(X, X^T, T) );
        db.Add( select(X, H^T, H^R) <<= select(X, T, R) );

        db.Add( repeat );
        db.Add( repeat <<= repeat );

        // rev/3 is an auxilary function for reverse/2
        PlgAtom rev("rev", 3, LibraryPredicate, false);

        db.Add( rev(Nil, X, X) <<= cut );
        db.Add( rev(H^T, R, X) <<= rev(T, H^R, X) );
        db.Add( reverse(L, R) <<= rev(L, Nil, R) );
    }

    PlgDatabase &GetDb()
    {
        static PlgDatabase db;
        static bool initialized = false;

        if (!initialized) {
            InitDb(db);
            initialized = true;
        }

        return db;
    }

    bool LibraryPredicate(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgClauseChoicePoint cp(PlgTerm(functor, args), cont, GetDb());
        cont.PushChoicePoint(cp);
        return false; //to force backtracking
    }

    PlgAtom nope("not", 1, LibraryPredicate, false);

    PlgAtom append("append", 3, LibraryPredicate, false);
    PlgAtom length("length", 2, LibraryPredicate, false);
    PlgAtom member("member", 2, LibraryPredicate, false);
    PlgAtom nth("nth", 3, LibraryPredicate, false);
    PlgAtom nth0("nth0", 3, LibraryPredicate, false);
    PlgAtom permutation("permutation", 2, LibraryPredicate, false);
    PlgAtom repeat("repeat", 0, LibraryPredicate, false);
    PlgAtom reverse("reverse", 2, LibraryPredicate, false);
    PlgAtom select("select", 3, LibraryPredicate, false);
}
