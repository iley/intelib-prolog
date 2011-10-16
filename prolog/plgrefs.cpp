#include "prolog.hpp"

// Prolog disjunction
PlgDisjunction::PlgDisjunction() : Super() {}
PlgDisjunction::PlgDisjunction(const PlgConjunction &head) : Super(new PlgDisjunctionImpl(head)) {}
PlgDisjunction::PlgDisjunction(const PlgConjunction &head, const PlgDisjunction &tail) : Super(new PlgDisjunctionImpl(head, tail)) {}

const PlgConjunction &PlgDisjunction::GetHead() const { return (*this)->GetHead(); }
const PlgDisjunction &PlgDisjunction::GetTail() const { return (*this)->GetTail(); }
PlgDisjunction PlgDisjunction::Append(const PlgDisjunction &rest) const { return (*this)->Append(rest); }

// Prolog conjunction
PlgConjunction::PlgConjunction() : Super() {}
PlgConjunction::PlgConjunction(const PlgTerm &head) : Super(new PlgConjunctionImpl(head)) {}
PlgConjunction::PlgConjunction(const PlgTerm &head, const PlgConjunction &tail) : Super(new PlgConjunctionImpl(head, tail)) {}

const PlgTerm &PlgConjunction::GetHead() const { return (*this)->GetHead(); }
const PlgConjunction &PlgConjunction::GetTail() const { return (*this)->GetTail(); }
PlgConjunction PlgConjunction::Append(const PlgConjunction &rest) const { return (*this)->Append(rest); }
