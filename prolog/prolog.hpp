#ifndef INTELIB_PROLOG_HPP_SENTRY
#define INTELIB_PROLOG_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/squeue.hpp"
#include "../sexpress/gensref.hpp"

#if INTELIB_TEXT_REPRESENTATIONS == 1
#include "../sexpress/sstring.hpp"
#endif

class PlgReference;
class PlgContext;
class PlgExpression : public SExpression
{
public:
    static IntelibTypeId TypeId;

    PlgExpression(const IntelibTypeId &typeId = TypeId) : SExpression(typeId) {}
    bool Unify(const PlgReference &other, PlgContext &context);

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
    PlgReference(const PlgReference &ref) : PlgRef(ref) {}
    PlgReference(const SReference &ref) : PlgRef(ref) {}
    PlgReference(SExpression *p) : PlgRef(p) {}

    ~PlgReference() {}
};

class PlgContext
{
};

class PlgDatabase;
class PlgContinuation
{
public:
    PlgContinuation(const PlgDatabase &db, const PlgReference &request) : database(db), queue() {
        queue.Append(request);
    }

    bool Next();
    PlgReference GetValue(const PlgReference &var);

private:
    const PlgDatabase &database;
    SQueue queue;
    SReference choicePoints;
    PlgContext context;
};

class PlgChoicePoint : public PlgExpression
{
public:
    virtual bool Next(PlgContext &context, SQueue &queue) = 0;
};

class PlgClauseChoicePoint : public PlgChoicePoint
{
public:
    PlgClauseChoicePoint(const SReference &cs, const PlgReference &cl) : candidates(cs), clause(cl) {}
    virtual bool Next(PlgContext &context, SQueue &executionQueue);
private:
    SReference candidates;
    PlgReference clause;
};

class PlgDatabase
{
public:
    PlgDatabase() : clauses(*PTheEmptyList) {}

    void Add(const PlgReference &clause);
    PlgContinuation Query(const PlgReference &request);
private:
    SReference clauses;
    PlgDatabase(const PlgDatabase &other);
};

class PlgClause : public PlgExpression
{
public:
    PlgClause(const PlgReference &hd, const PlgReference &bd) : head(hd), body(bd) {}
    PlgReference Head() const { return head; }
    PlgReference Body() const { return body; }

private:
    PlgReference head;
    PlgReference body;
};

class PlgAtom : public PlgExpression
{
public:
    PlgAtom(const char *name) : label(new SExpressionLabel(name)) {}
    const SReference Label() const { return label; }
    const char *GetName() const { return label.DynamicCastGetPtr<SExpressionLabel>()->GetName(); }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

private:
    SReference label;
};

#endif
