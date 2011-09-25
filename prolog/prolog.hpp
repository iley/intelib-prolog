#ifndef INTELIB_PROLOG_HPP_SENTRY
#define INTELIB_PROLOG_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/gensref.hpp"
#include "../sexpress/custmem.hpp"

class IntelibX_not_a_prolog_term : public IntelibX
{
public:
    IntelibX_not_a_prolog_term(SReference a_param);
};

class PlgTerm;
typedef GenericSReference<PlgTerm, IntelibX_not_a_prolog_term> PlgTermRef;

class PlgTermReference : public PlgTermRef {
public:
    PlgTermReference() {}
    PlgTermReference(const PlgTermReference& ref) : PlgTermRef(ref) {}
    PlgTermReference(const SReference& ref) : PlgTermRef(ref) {}
    PlgTermReference(SExpression* p) : PlgTermRef(p) {}

    ~PlgTermReference() {}
};

class PlgTerm : public SExpression {
public:
    static IntelibTypeId TypeId;
};

#endif //INTELIB_PROLOG_HPP_SENTRY
