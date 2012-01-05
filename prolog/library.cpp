#include "library.hpp"
#include "utils.hpp"

static SListConstructor S;

bool PlgDefaultPredicate(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
    PlgClauseChoicePoint cp(PlgTerm(functor, args), cont);
    cont.PushChoicePoint(cp);
    return false; //to force backtracking
}

// Conjunction

bool PredicateConj(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
    PlgReference left = args.Car();
    PlgReference right = args.Cdr().Car();
    cont.PushQuery(right);
    cont.PushQuery(left);
    return true;
}

PlgAtom PlgAtomConj(", ", 2, PredicateConj, true);

// Disjunction

bool PredicateDisj(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
    PlgDisjChoicePoint cp(args, cont);
    cont.PushChoicePoint(cp);
    return false; //to force backtracking
}

PlgAtom PlgAtomDisj("; ", 2, PredicateDisj, true);

// "=" predicate

bool PredicateEquals(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
    PlgReference left = args.Car();
    PlgReference right = args.Cdr().Car();

    return left.Unify(right, cont.Context());
}

PlgAtom PlgAtomEquals(" = ", 2, PredicateEquals, true);

bool PredicateTrue(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
    return true;
}

PlgAtom PlgTrue("true", 0, PredicateTrue, false);

bool PredicateNotUnifies(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
    int pos = cont.Context().Top();
    PlgReference left = args.Car(),
        right = args.Cdr().Car();
    bool result = left.Unify(right, cont.Context());
    cont.Context().ReturnTo(pos);
    return !result;
}

PlgAtom NotUnifies(" /= ", 2, PredicateNotUnifies, true);

PlgAtom PlgAtomMinus(" - ", 2);
PlgAtom PlgAtomPlus(" + ", 2);
PlgAtom PlgAtomMultiply(" * ", 2);
PlgAtom PlgAtomDivide(" / ", 2);

static int NumericEval(const PlgReference &expr) {
    INTELIB_ASSERT(expr.GetPtr(), IntelibX_unexpected_unbound_value());
    
    if (expr->TermType() == SExpressionInt::TypeId) {
        return expr.GetInt();
    } else if (expr->TermType() == PlgExpressionTerm::TypeId) {
        PlgTerm term = expr;
        PlgAtom oper = term->Functor();
        int left = NumericEval(term->Args().Car()),
            right = NumericEval(term->Args().Cdr().Car()),
            result;

        if (oper == PlgAtomMinus)
            result = left - right;
        else if (oper == PlgAtomPlus)
            result = left + right;
        else if (oper == PlgAtomMultiply)
            result = left * right;
        else if (oper == PlgAtomDivide)
            result = left / right;
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

    return left.Unify(SReference(NumericEval(right.Evaluate(cont.Context()))), cont.Context());
}

PlgAtom PlgAtomIs(" is ", 2, PredicateIs, true);

bool PredicateNumericEq(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
    PlgReference left = args.Car(),
        right = args.Cdr().Car();
    return NumericEval(left) == NumericEval(right);
}

PlgAtom PlgAtomNumericEq(" =:= ", PredicateNumericEq, true);

bool PredicateNumericNe(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont) {
    PlgReference left = args.Car(),
        right = args.Cdr().Car();
    return NumericEval(left) != NumericEval(right);
}

PlgAtom PlgAtomNumericNe(" =\= ", PredicateNumericNe, true);
