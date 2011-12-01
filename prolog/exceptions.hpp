#ifndef INTELIB_PROLOG_EXCEPTIONS_HPP_SENTRY
#define INTELIB_PROLOG_EXCEPTIONS_HPP_SENTRY

#include "../sexpress/iexcept.hpp"

class IntelibX_not_a_prolog_expression : public IntelibX
{
public:
    IntelibX_not_a_prolog_expression(SReference a_param);
};

class IntelibX_not_a_prolog_clause : public IntelibX
{
public:
    IntelibX_not_a_prolog_clause(SReference a_param);
};

class IntelibX_not_a_prolog_atom : public IntelibX
{
public:
    IntelibX_not_a_prolog_atom(SReference a_param);
};

class IntelibX_not_a_prolog_variable_name : public IntelibX
{
public:
    IntelibX_not_a_prolog_variable_name(SReference a_param);
};

class IntelibX_not_a_prolog_term : public IntelibX
{
public:
    IntelibX_not_a_prolog_term(SReference a_param);
};

class IntelibX_not_a_prolog_disjunction : public IntelibX
{
public:
    IntelibX_not_a_prolog_disjunction(SReference a_param);
};

class IntelibX_not_a_prolog_conjunction : public IntelibX
{
public:
    IntelibX_not_a_prolog_conjunction(SReference a_param);
};

#endif
