#include "prolog.hpp"

IntelibTypeId PlgExpression::TypeId(&SExpression::TypeId, true);

IntelibX_not_a_prolog_expression::IntelibX_not_a_prolog_expression(SReference a_param) 
    : IntelibX("Not a prolog expression", a_param) {}

void PlgDatabase::Add(const PlgReference &clause) {
    clauses = clauses.MakeCons(clause);
}
