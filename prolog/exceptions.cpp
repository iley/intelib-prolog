#include "exceptions.hpp"

IntelibX_not_a_prolog_expression::IntelibX_not_a_prolog_expression(SReference a_param)
    : IntelibX("Not a prolog expression", a_param) {}

IntelibX_not_a_prolog_clause::IntelibX_not_a_prolog_clause(SReference a_param)
    : IntelibX("Not a prolog clause", a_param) {}

IntelibX_not_a_prolog_atom::IntelibX_not_a_prolog_atom(SReference a_param)
    : IntelibX("Not a prolog atom", a_param) {}

IntelibX_not_a_prolog_variable_name::IntelibX_not_a_prolog_variable_name(SReference a_param)
    : IntelibX("Not a prolog variable name", a_param) {}

IntelibX_not_a_prolog_term::IntelibX_not_a_prolog_term(SReference a_param)
    : IntelibX("Not a prolog term", a_param) {}

IntelibX_not_a_prolog_disjunction::IntelibX_not_a_prolog_disjunction(SReference a_param)
    : IntelibX("Not a prolog disjunction", a_param) {}

IntelibX_not_a_prolog_conjunction::IntelibX_not_a_prolog_conjunction(SReference a_param)
    : IntelibX("Not a prolog conjunction", a_param) {}

IntelibX_not_a_prolog_continuation::IntelibX_not_a_prolog_continuation(SReference a_param)
    : IntelibX("Not a prolog continuation", a_param) {}
