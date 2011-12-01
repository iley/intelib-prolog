#ifndef INTELIB_PROLOG_HPP_SENTRY
#define INTELIB_PROLOG_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/squeue.hpp"
#include "../sexpress/gensref.hpp"

class PlgExpression : public SExpression
{
public:
    static IntelibTypeId TypeId;

    PlgExpression(const IntelibTypeId &typeId = TypeId) : SExpression(typeId) {}

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif
};

class IntelibX_not_a_prolog_expression : public IntelibX
{
public:
    IntelibX_not_a_prolog_expression(SReference a_param);
};

typedef GenericSReference<PlgExpression, IntelibX_not_a_prolog_expression> PlgRef;

class PlgReference : public PlgRef
{
public:
    PlgReference() {}
    PlgReference(const PlgReference& ref) : PlgRef(ref) {}
    PlgReference(const SReference& ref) : PlgRef(ref) {}
    PlgReference(SExpression* p) : PlgRef(p) {}

    ~PlgReference() {}
};

class PlgContext
{
};

class PlgContinuation
{
public:
    bool Next();
    PlgReference GetValue(const PlgReference &var);

private:
    SQueue clauses;
    PlgContext context;
};

class PlgDatabase
{
public:
    PlgDatabase() : clauses(*PTheEmptyList) {}

    void Add(const PlgReference& clause);
    PlgContinuation Query(const PlgReference& request);
private:
    SReference clauses;
    PlgDatabase(const PlgDatabase& other);
};

#endif
