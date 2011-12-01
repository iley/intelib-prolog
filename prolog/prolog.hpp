#ifndef INTELIB_PROLOG_HPP_SENTRY
#define INTELIB_PROLOG_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/squeue.hpp"
#include "../sexpress/gensref.hpp"

#include "exceptions.hpp"

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

class PlgAtomExpression : public PlgExpression
{
    friend class PlgAtom;
public:
    static IntelibTypeId TypeId;

    PlgAtomExpression(const char *name) : PlgExpression(TypeId), label(name) {}
    const SReference Label() const { return label; }
    const char *GetName() const { return label.GetPtr()->GetName(); }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

protected:
    PlgAtomExpression(const IntelibTypeId &typeId, const char *name) : PlgExpression(typeId), label(name) {}

private:
    SLabel label;
};

typedef GenericSReference<PlgAtomExpression, IntelibX_not_a_prolog_atom> PlgAtom_Super;

class PlgAtom : public PlgAtom_Super
{
public:
    PlgAtom(const char *name) : PlgAtom_Super(new PlgAtomExpression(name)) {}
};

class PlgVariableNameExpression : public PlgAtomExpression
{
public:
    static IntelibTypeId TypeId;

    PlgVariableNameExpression(const char *name) : PlgAtomExpression(TypeId, name) {}
};

class PlgTermExpression : public PlgExpression
{
public:
    static IntelibTypeId TypeId;

    PlgTermExpression(const PlgAtom &fn, const SReference &as);

    const PlgAtom &Functor() const { return functor; }
    const SReference &Args() const { return args; }
    const int Arity() const { return arity; }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

private:
    PlgAtom functor;
    SReference args;
    int arity;
};

typedef GenericSReference<PlgTermExpression, IntelibX_not_a_prolog_atom> PlgTerm_Super;

class PlgTerm : public PlgTerm_Super
{
public:
    PlgTerm(const PlgAtom &functor, const SReference &args) : PlgTerm_Super(new PlgTermExpression(functor, args)) {}
};

#endif
