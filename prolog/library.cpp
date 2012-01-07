#include "library.hpp"
#include "utils.hpp"

bool PlgDefaultPredicate(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
    PlgClauseChoicePoint cp(PlgTerm(functor, args), cont);
    cont.PushChoicePoint(cp);
    return false; //to force backtracking
}

namespace PlgStdLib {
    static SListConstructor S;

    // Conjunction

    bool PredicateConjunction(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
        PlgReference left = args.Car();
        PlgReference right = args.Cdr().Car();
        cont.PushQuery(right);
        cont.PushQuery(left);
        return true;
    }

    PlgAtom conjunction(", ", 2, PredicateConjunction, true);

    // Disjunction

    bool PredicateDisjunction(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
        PlgDisjChoicePoint cp(args, cont);
        cont.PushChoicePoint(cp);
        return false; //to force backtracking
    }

    PlgAtom disjunction("; ", 2, PredicateDisjunction, true);

    // "=" predicate

    bool PredicateUnifies(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
        PlgReference left = args.Car();
        PlgReference right = args.Cdr().Car();

        return left.Unify(right, cont.Context());
    }

    PlgAtom unifies(" = ", 2, PredicateUnifies, true);

    // Truth value
    bool PredicateTrue(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
        return true;
    }

    PlgAtom truth("true", 0, PredicateTrue, false);

    // "/=" predicate

    bool PredicateNotUnifies(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
        int pos = cont.Context().Top();
        PlgReference left = args.Car(),
                     right = args.Cdr().Car();
        bool result = left.Unify(right, cont.Context());
        cont.Context().ReturnTo(pos);
        return !result;
    }

    PlgAtom not_unifies(" /= ", 2, PredicateNotUnifies, true);

    // Cut

    bool PredicateCut(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
        cont.ResetChoicePoints();
        return true;
    }

    PlgAtom cut("!", 0, PredicateCut, false);

    // Integer arithmetic

    PlgAtom minus(" - ", 2);
    PlgAtom plus(" + ", 2);
    PlgAtom multiply(" * ", 2);
    PlgAtom divide(" / ", 2);
    PlgAtom reminder(" % ", 2);

    static int IntEval(const PlgReference &expr) {
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

    bool PredicateIs(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
        PlgReference left = args.Car(),
                     right = args.Cdr().Car();

        return left.Unify(SReference(IntEval(right.Evaluate(cont.Context()))), cont.Context());
    }

    PlgAtom is(" is ", 2, PredicateIs, true);

    bool PredicateIntEqual(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
        PlgReference left = args.Car(),
                     right = args.Cdr().Car();
        return IntEval(left) == IntEval(right);
    }

    PlgAtom int_equal(" =:= ", PredicateIntEqual, true);

    bool PredicateIntNotEqual(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
        PlgReference left = args.Car(),
                     right = args.Cdr().Car();
        return IntEval(left) != IntEval(right);
    }

    PlgAtom int_not_equal(" =\= ", PredicateIntNotEqual, true);

    bool PredicateIntLess(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
        PlgReference left = args.Car(),
                     right = args.Cdr().Car();
        return IntEval(left) < IntEval(right);
    }

    PlgAtom int_less(" < ", 2, PredicateIntLess, true);

    bool PredicateIntLessOrEqual(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
        PlgReference left = args.Car(),
                     right = args.Cdr().Car();
        return IntEval(left) <= IntEval(right);
    }

    PlgAtom int_less_or_equal(" <= ", 2, PredicateIntLessOrEqual, true);

    bool PredicateIntGreater(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
        PlgReference left = args.Car(),
                     right = args.Cdr().Car();
        return IntEval(left) > IntEval(right);
    }

    PlgAtom int_greater(" > ", 2, PredicateIntGreater, true);;

    bool PredicateIntGreaterOrEqual(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
        PlgReference left = args.Car(),
                     right = args.Cdr().Car();
        return IntEval(left) >= IntEval(right);
    }

    PlgAtom int_greater_or_equal(" >= ", 2, PredicateIntGreaterOrEqual);
}
