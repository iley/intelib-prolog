#ifndef INTELIB_PROLOG_ENGINE_HPP_SENTRY
#define INTELIB_PROLOG_ENGINE_HPP_SENTRY

#include "data.hpp"
#include "../sexpress/sexpress.hpp"
#include "../sexpress/squeue.hpp"
#include "../sexpress/svector.hpp"
#include "../sexpress/gensref.hpp"
#include "../sexpress/shashtbl.hpp"

class PlgContext
{
public:
    PlgContext() : values(), top(0)
    {
        frameVars[0] = *PTheEmptyList;
    }

    void Set(const PlgReference &var, const PlgReference &value)
    {
        values->AddItem(var, value);
        frameVars[top] = var ^ frameVars[top];
    }

    PlgReference Get(const PlgReference &var) const
    {
        return values->FindItem(var, PlgUnbound);
    }

    void ReturnTo(int pos, bool merge = false);

    int Top() const { return top; }

    int NextFrame()
    {
        top++;
        frameVars[top] = *PTheEmptyList;
        return top - 1;
    }

    const SHashTable &ValueTable() const
    {
        return values;
    }

private:
    SHashTable values;
    SVector frameVars;
    int top;

    PlgContext(const PlgContext&);
};

class PlgDatabase;
class PlgExpressionContinuation : public SExpression
{
    friend class PlgDatabase;
    friend class PlgExpressionChoicePoint;
    friend class PlgExpressionClauseChoicePoint;
    friend class PlgExpressionDisjChoicePoint;
public:
    static IntelibTypeId TypeId;

    PlgExpressionContinuation(PlgDatabase &db, const PlgReference &req);

    bool Next();
    PlgReference GetValue(const PlgReference &var);

    void PushChoicePoint(const PlgReference &point);
    void PopChoicePoint();
    void ResetChoicePoints();

    PlgDatabase &Database() { return database; }
    SReference ChoicePoints() { return choicePoints; }
    PlgContext &Context() { return context; }

    void PushQuery(const PlgReference &query);

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif


private:
    PlgDatabase &database;
    SReference choicePoints;
    PlgContext context;
    SHashTable queryVars;
    SReference queries;

    bool Backtrack();
};

typedef GenericSReference<PlgExpressionContinuation, IntelibX_not_a_prolog_continuation> PlgContinuation_Super;

class PlgContinuation : public PlgContinuation_Super
{
public:
    PlgContinuation(PlgDatabase &db, const PlgReference &request) : PlgContinuation_Super(new PlgExpressionContinuation(db, request)) {}
};

// database

class PlgDatabase
{
public:
    PlgDatabase() : table() {}

    void AddA(const PlgReference &clause); //add to the head of list
    void Add(const PlgReference &clause);
    void AddWithoutExpansion(const PlgReference &clause);

    PlgContinuation Query(const PlgReference &request);
    SReference Head(const PlgReference &functor) const { return table->FindItem(functor, *PTheEmptyList); }
private:
    SHashTable table;
    PlgDatabase(const PlgDatabase &other);

    PlgReference Clause(const PlgReference &ref) const;
    PlgReference ExpandTerm(const PlgReference &ref);
};

// choice points

class PlgExpressionChoicePoint : public SExpression
{
public:
    static IntelibTypeId TypeId;

    virtual bool TryNext() = 0;
    void Restore();

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

protected:
    PlgExpressionContinuation &cont;
    int contextPos;
    SReference queries;

    PlgExpressionChoicePoint(const IntelibTypeId &typeId, PlgExpressionContinuation &c)
        : SExpression(typeId), cont(c), contextPos(c.context.NextFrame()), queries(c.queries) {}
};

typedef GenericSReference<PlgExpressionChoicePoint, IntelibX_not_a_prolog_choice_point> PlgChoicePoint;

class PlgExpressionClauseChoicePoint : public PlgExpressionChoicePoint
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionClauseChoicePoint(const PlgReference &cls, PlgExpressionContinuation &c, PlgDatabase &db)
        : PlgExpressionChoicePoint(TypeId, c), clause(cls), pointer(db.Head(cls.Functor())) {}

    virtual bool TryNext();
private:
    PlgReference clause;
    SReference pointer;
};

typedef GenericSReference<PlgExpressionClauseChoicePoint, IntelibX_not_a_prolog_clause_choice_point> PlgClauseChoicePoint_Super;

class PlgClauseChoicePoint : public PlgClauseChoicePoint_Super
{
public:
    PlgClauseChoicePoint(const PlgReference &clause, PlgExpressionContinuation &cont)
        : PlgClauseChoicePoint_Super(new PlgExpressionClauseChoicePoint(clause, cont, cont.Database())) {}

    PlgClauseChoicePoint(const PlgReference &clause, PlgExpressionContinuation &cont, PlgDatabase &db)
        : PlgClauseChoicePoint_Super(new PlgExpressionClauseChoicePoint(clause, cont, db)) {}
};

class PlgExpressionDisjChoicePoint : public PlgExpressionChoicePoint
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionDisjChoicePoint(const SReference &vs, PlgExpressionContinuation &cont)
        : PlgExpressionChoicePoint(TypeId, cont), variants(vs) {}

    virtual bool TryNext();

private:
    PlgReference variants;
};

typedef GenericSReference<PlgExpressionDisjChoicePoint, IntelibX_not_a_prolog_clause_choice_point> PlgDisjChoicePoint_Super;

class PlgDisjChoicePoint : public PlgDisjChoicePoint_Super
{
public:
    PlgDisjChoicePoint(const SReference &variants, PlgExpressionContinuation &cont)
        : PlgDisjChoicePoint_Super(new PlgExpressionDisjChoicePoint(variants, cont)) {}
};

#endif
