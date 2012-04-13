#include "plexceptions.hpp"

IntelibX_not_a_prolog_object::IntelibX_not_a_prolog_object(SReference a_param)
    : IntelibX("Not a prolog object", a_param) {}

IntelibX_not_a_prolog_clause::IntelibX_not_a_prolog_clause(SReference a_param)
    : IntelibX("Not a prolog clause", a_param) {}

IntelibX_not_a_prolog_atom::IntelibX_not_a_prolog_atom(SReference a_param)
    : IntelibX("Not a prolog atom", a_param) {}

IntelibX_not_a_prolog_variable_name::IntelibX_not_a_prolog_variable_name(SReference a_param)
    : IntelibX("Not a prolog variable name", a_param) {}

IntelibX_not_a_prolog_variable_index::IntelibX_not_a_prolog_variable_index(SReference a_param)
    : IntelibX("Not a prolog variable index", a_param) {}

IntelibX_not_a_prolog_term::IntelibX_not_a_prolog_term(SReference a_param)
    : IntelibX("Not a prolog term", a_param) {}

IntelibX_not_a_prolog_continuation::IntelibX_not_a_prolog_continuation(SReference a_param)
    : IntelibX("Not a prolog continuation", a_param) {}

IntelibX_not_a_prolog_choice_point::IntelibX_not_a_prolog_choice_point(SReference a_param)
    : IntelibX("Not a prolog choice point", a_param) {}

IntelibX_not_a_prolog_clause_choice_point::IntelibX_not_a_prolog_clause_choice_point(SReference a_param)
    : IntelibX("Not a prolog clause choice point", a_param) {}

IntelibX_not_a_prolog_truth::IntelibX_not_a_prolog_truth(SReference a_param)
    : IntelibX("Not a prolog truth value", a_param) {}

IntelibX_not_a_prolog_predicate::IntelibX_not_a_prolog_predicate(SReference a_param)
    : IntelibX("Not a prolog predicate", a_param) {}

IntelibX_not_a_prolog_sentence_mark::IntelibX_not_a_prolog_sentence_mark(SReference a_param)
    : IntelibX("Not a prolog sentence mark", a_param) {}

IntelibX_not_a_prolog_anonymous_variable::IntelibX_not_a_prolog_anonymous_variable(SReference a_param)
    : IntelibX("Not a prolog anonymous variable", a_param) {}

IntelibX_invalid_arity::IntelibX_invalid_arity(int arity)
    : IntelibX("Invalid arity", arity) {}

IntelibX_prolog_error::IntelibX_prolog_error(SReference a_param)
    : IntelibX("Prolog error", a_param) {}
