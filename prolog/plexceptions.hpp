#ifndef INTELIB_PROLOG_EXCEPTIONS_HPP_SENTRY
#define INTELIB_PROLOG_EXCEPTIONS_HPP_SENTRY

#include "../sexpress/iexcept.hpp"

class IntelibX_not_a_prolog_object : public IntelibX
{
public:
    IntelibX_not_a_prolog_object(SReference a_param);
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

class IntelibX_not_a_prolog_variable_index : public IntelibX
{
public:
    IntelibX_not_a_prolog_variable_index(SReference a_param);
};

class IntelibX_not_a_prolog_term : public IntelibX
{
public:
    IntelibX_not_a_prolog_term(SReference a_param);
};

class IntelibX_not_a_prolog_continuation : public IntelibX
{
public:
    IntelibX_not_a_prolog_continuation(SReference a_param);
};

class IntelibX_not_a_prolog_choice_point : public IntelibX
{
public:
    IntelibX_not_a_prolog_choice_point(SReference a_param);
};

class IntelibX_not_a_prolog_clause_choice_point : public IntelibX
{
public:
    IntelibX_not_a_prolog_clause_choice_point(SReference a_param);
};

class IntelibX_not_a_prolog_truth : public IntelibX
{
public:
    IntelibX_not_a_prolog_truth(SReference a_param);
};

class IntelibX_not_a_prolog_predicate : public IntelibX
{
public:
    IntelibX_not_a_prolog_predicate(SReference a_param);
};

class IntelibX_not_a_prolog_sentence_mark : public IntelibX
{
public:
    IntelibX_not_a_prolog_sentence_mark(SReference a_param);
};

class IntelibX_not_a_prolog_anonymous_variable : public IntelibX
{
public:
    IntelibX_not_a_prolog_anonymous_variable(SReference a_param);
};

class IntelibX_invalid_arity : public IntelibX
{
public:
    IntelibX_invalid_arity(int value);
};

class IntelibX_prolog_error : public IntelibX
{
public:
    IntelibX_prolog_error(SReference a_param);
};

#endif
