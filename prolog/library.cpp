#include "library.hpp"
#include "utils.hpp"

static SListConstructor S;

// Disjunction

IntelibTypeId PlgExpressionDisjunction::TypeId(&PlgExpressionTerm::TypeId, false);

PlgAtom PlgExpressionDisjunction::Atom("<OR>");

PlgReference PlgExpressionDisjunction::RenameVars(const PlgReference &self, NameGeneratorFunction nameGenerator, SHashTable &existingVars) const {
    SReference newList = *PTheEmptyList;
    for (SReference p = args; !p.IsEmptyList(); p = p.Cdr()) {
        PlgReference pref = p.Car();
        newList.AddAnotherItemToList(pref.RenameVars(nameGenerator, existingVars));
    }
    return PlgDisjunction(newList);
}

bool PlgExpressionDisjunction::Solve(const PlgReference &self, PlgExpressionContinuation &continuation) const {
    //TODO
    throw IntelibX_not_implemented();
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionDisjunction::TextRepresentation() const {
    return Join("; ", args);
}
#endif

PlgDisjunction operator | (const PlgReference &left, const PlgReference &right) {
    return PlgDisjunction((S| left, right ));
}

// Conjunction

IntelibTypeId PlgExpressionConjunction::TypeId(&PlgExpressionTerm::TypeId, false);

PlgAtom PlgExpressionConjunction::Atom("<OR>");

bool PlgExpressionConjunction::Solve(const PlgReference &self, PlgExpressionContinuation &continuation) const {
    //TODO
    throw IntelibX_not_implemented();
}

PlgReference PlgExpressionConjunction::RenameVars(const PlgReference &self, NameGeneratorFunction nameGenerator, SHashTable &existingVars) const {
    SReference newList = *PTheEmptyList;
    for (SReference p = args; !p.IsEmptyList(); p = p.Cdr()) {
        PlgReference pref = p.Car();
        newList.AddAnotherItemToList(pref.RenameVars(nameGenerator, existingVars));
    }
    return PlgConjunction(newList);
}

#if INTELIB_TEXT_REPRESENTATIONS == 1
SString PlgExpressionConjunction::TextRepresentation() const {
    return Join(", ", args);
}
#endif

PlgConjunction operator & (const PlgReference &left, const PlgReference &right) {
    return PlgConjunction((S| left, right ));
}

// "==" predicate

bool PredicateEquals(const SReference &args, PlgExpressionContinuation &cont) {
    PlgReference left = args.Car();
    PlgReference right = args.Cdr().Car();

    return left.Unify(right, cont.Context());
}

PlgAtom PlgAtomEquals("=", 2, PredicateEquals);
