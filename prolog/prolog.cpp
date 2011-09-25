#include "prolog.hpp"

IntelibX_not_a_prolog_term::
IntelibX_not_a_prolog_term(SReference a_param) 
    : IntelibX("Not a prolog term", a_param) {}

IntelibTypeId PlgTerm::TypeId(&SExpression::TypeId, true);
