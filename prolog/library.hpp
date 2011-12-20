#ifndef INTELIB_PROLOG_LIBRARY_HPP_SENTRY
#define INTELIB_PROLOG_LIBRARY_HPP_SENTRY
#include "data.hpp"

// Disjunction
class PlgExpressionDisjunction : public PlgExpressionTerm
{
public:
    static IntelibTypeId TypeId;
    static PlgAtom Atom;

    explicit PlgExpressionDisjunction(const SReference &ls) : PlgExpressionTerm(TypeId, Atom, ls) {}

    virtual bool Solve(const PlgReference &self, PlgExpressionContinuation &continuation) const;
    virtual PlgReference RenameVars(const PlgReference &self, NameGeneratorFunction nameGenerator, SHashTable &existingVars) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif
};

typedef GenericSReference<PlgExpressionDisjunction, IntelibX_not_a_prolog_disjunction> PlgDisjunction_Super;

class PlgDisjunction : public PlgDisjunction_Super
{
public:
    explicit PlgDisjunction(const SReference &ls) : PlgDisjunction_Super(new PlgExpressionDisjunction(ls)) {}
};

PlgDisjunction operator | (const PlgReference &left, const PlgReference &right);

// Conjunction

class PlgExpressionConjunction : public PlgExpressionTerm
{
public:
    static IntelibTypeId TypeId;
    static PlgAtom Atom;

    explicit PlgExpressionConjunction(const SReference &ls) : PlgExpressionTerm(TypeId, Atom, ls) {}

    virtual bool Solve(const PlgReference &self, PlgExpressionContinuation &continuation) const;
    virtual PlgReference RenameVars(const PlgReference &self, NameGeneratorFunction nameGenerator, SHashTable &existingVars) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif
};

typedef GenericSReference<PlgExpressionConjunction, IntelibX_not_a_prolog_conjunction> PlgConjunction_Super;

class PlgConjunction : public PlgConjunction_Super
{
public:
    explicit PlgConjunction(const SReference &ls) : PlgConjunction_Super(new PlgExpressionConjunction(ls)) {}
};

PlgConjunction operator & (const PlgReference &left, const PlgReference &right);

// Unification term

extern PlgAtom PlgAtomEquals;

inline PlgReference operator ^= (const PlgReference &left, const PlgReference &right) {
    return PlgAtomEquals(left, right);
}

#endif
