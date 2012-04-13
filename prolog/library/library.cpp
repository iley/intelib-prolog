#include "../../sexpress/shashtbl.hpp"

#include <stdio.h>

#include "../plutils.hpp"
#include "../plengine.hpp"
#include "../plexceptions.hpp"

#include "library.hpp"

namespace PlgStdLib
{
    static SListConstructor S;

    PlgAnonymousVariable _;

    PlgAtom action("{}", 1, PlgDefaultPredicate, false);
    PlgAtom dcg_translate_rule("dcg_translate_rule", 2, PlgDefaultPredicate, false);
    PlgAtom phrase("phrase", PlgDefaultPredicate, false);

    bool PredicateListing(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        //FIXME
        //SString str = cont.Database().Dump();
        //printf("%s", str.c_str());
        return true;
    }

    PlgAtom listing("listing", 0, PredicateListing, false);

    bool PredicateError(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        throw IntelibX_prolog_error(args.Car());
    }

    PlgAtom error("error", PredicateError, false);

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

    PlgAtom implication(":-", 2, PlgDefaultPredicate, true);

    PlgAtom rightarrow("-->", 2, PlgDefaultPredicate, true);

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
        cont.Cut();
        return true;
    }

    PlgAtom cut("!", 0, PredicateCut, false);

    // =../2
    bool PredicateUniv(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgReference term = args.Car(),
                     list = args.Cdr().Car();

        if (term->TermType() == PlgExpressionVariable::TypeId)
            return term.Unify(PlgTerm(list.Car(), list.Cdr()), cont.Context());
        else 
            return list.Unify(term.Functor() ^ term.Args(), cont.Context());
    }

    PlgAtom univ("univ", 2, PredicateUniv, false);

    // Assert

    bool PredicateAssertA(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgReference arg = args.Car();
        AssertA(arg);
        return true;
    }

    PlgAtom assert("assert", 1, PredicateAssertA, false);
    PlgAtom asserta("asserta", 1, PredicateAssertA, false);

    bool PredicateAssertZ(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgReference arg = args.Car();
        Assert(arg);
        return true;
    }

    PlgAtom assertz("assertz", 1, PredicateAssertZ, false);

    // Retract

    bool PredicateRetract(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgReference arg = args.Car();
        //return cont.Database().Retract(arg, cont.Context());
        //FIXME
        return false;
    }

    PlgAtom retract("retract", 1, PredicateRetract, false);

    // Integer arithmetic

    PlgAtom minus("-", true);
    PlgAtom plus("+", true);
    PlgAtom multiply("*", true);
    PlgAtom divide("/", true);
    PlgAtom reminder("%", true);
    PlgAtom power("^", true); //TODO

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

            if (oper.IsEql(minus))
                result = left - right;
            else if (oper.IsEql(plus))
                result = left + right;
            else if (oper.IsEql(multiply))
                result = left * right;
            else if (oper.IsEql(divide))
                result = left / right;
            else if (oper.IsEql(reminder))
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

    // Type checking

    bool PredicateAtom(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        return args.Car()->TermType() == PlgExpressionAtom::TypeId;
    }

    PlgAtom atom("atom", 1, PredicateAtom);

    //FIXME: atomic/1 now returns true for everything except lists, compund terms and vars
    bool PredicateAtomic(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgReference arg = args.Car();
        const IntelibTypeId &type = arg->TermType();
        return type != PlgExpressionTerm::TypeId
            && type != PlgExpressionVariable::TypeId
            && type != SExpressionCons::TypeId
            && !arg.IsEmptyList();
    }

    PlgAtom atomic("atomic", 1, PredicateAtomic);

    bool PredicateCompund(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        return args.Car()->TermType() == PlgExpressionTerm::TypeId;
    }

    PlgAtom compound("compound", 1, PredicateCompund);

    bool PredicateInteger(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgReference arg = args.Car();
        return arg->TermType() == SExpressionInt::TypeId
            || arg->TermType() == SExpressionChar::TypeId;
    }

    PlgAtom integer("integer", 1, PredicateInteger);

    bool PredicateNonVar(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        return args.Car()->TermType() != PlgExpressionVariable::TypeId;
    }

    PlgAtom nonvar("nonvar", 1, PredicateNonVar);

    bool PredicateVar(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        return args.Car()->TermType() == PlgExpressionVariable::TypeId;
    }

    PlgAtom var("var", 1, PredicateVar);

    // Database with standard predicates written in prolog

    void Init()
    {
        static PlgVariable X("X"), H("H"), T("T"), L("L"), R("R"), N("N"), N1("N1");
        static SReference &Nil = *(GetEmptyList());

        static bool done = false;

        if (done)
            return;
        done = true;

        PlgVariable Term("Term"), Result("Result");

        //TODO: move to prolog module
        AssertWithoutExpansion( expand_term(Term, Result) << (dcg_translate_rule(Term, Result) & cut) );
        AssertWithoutExpansion( expand_term(Term, Result) << (term_expansion(Term, Result) & cut) );
        AssertWithoutExpansion( expand_term(Term, Term) );

        grammar::Init();
        output::Init();

        nope(X) <<= (X & cut & fail) | truth; // not(X) :- X, !, fail; true

        *append(Nil, X, X);
        append(H^T, L, H^R) <<= append(T, L, R);

        member(X, H^T) <<= (X ^= H) | member(X, T);

        length(Nil, 0) <<= cut;
        length(H^T, N) <<= length(T,N1) & N.is(N1 + SReference(1));

        PlgAtom index("index", 4, PlgDefaultPredicate, false);
        PlgVariable StartIndex("StartIndex");

        // index/4 is an auxilary predicate to implement nth and nth0
        index(StartIndex, N, L, R) <<= (N < StartIndex) & cut & fail;
        index(StartIndex, StartIndex, H^T, H) <<= cut;
        index(StartIndex, N, H^T, X) <<= N1.is(N - SReference(1)) & index(StartIndex, N1, T, X);

        nth(N, L, X) <<= index(1, N, L, X);
        nth0(N, L, X) <<= index(0, N, L, X);

        *permutation(Nil, Nil);
        permutation(H^T, R) <<= permutation(T,L) & select(H,R,L);

        *select(X, X^T, T);
        select(X, H^T, H^R) <<= select(X, T, R);

        *repeat;
        repeat <<= repeat;

        // rev/3 is an auxilary function for reverse/2
        PlgAtom rev("rev", 3, PlgDefaultPredicate, false);

        rev(Nil, X, X) <<= cut;
        rev(H^T, R, X) <<= rev(T, H^R, X);
        reverse(L, R) <<= rev(L, Nil, R);
    }

    PlgAtom nope("not", 1, PlgDefaultPredicate, false);

    PlgAtom append("append", 3, PlgDefaultPredicate, false);
    PlgAtom length("length", 2, PlgDefaultPredicate, false);
    PlgAtom member("member", 2, PlgDefaultPredicate, false);
    PlgAtom nth("nth", 3, PlgDefaultPredicate, false);
    PlgAtom nth0("nth0", 3, PlgDefaultPredicate, false);
    PlgAtom permutation("permutation", 2, PlgDefaultPredicate, false);
    PlgAtom repeat("repeat", 0, PlgDefaultPredicate, false);
    PlgAtom reverse("reverse", 2, PlgDefaultPredicate, false);
    PlgAtom select("select", 3, PlgDefaultPredicate, false);

    PlgAtom expand_term("expand_term", 2, PlgDefaultPredicate, false);
    PlgAtom term_expansion("term_expansion", 2, PlgDefaultPredicate, false);
    PlgAtom goal_expansion("goal_expansion", 2, PlgDefaultPredicate, false);
}

bool PlgDefaultPredicate(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
{
    PlgStdLib::Init();
    PlgClauseChoicePoint cp(PlgTerm(functor, args), cont);
    cont.PushChoicePoint(cp);
    return false; //to force backtracking
}
