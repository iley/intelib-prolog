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
    PlgContext() : values(), top(0) {}

    void Set(const PlgReference &index, const PlgReference &value) {
        values[indexValue(index)] = value;
    }

    PlgReference Get(const PlgReference &index) const {
        return values[indexValue(index)];
    }

    PlgReference Evaluate(const PlgReference &value) const;

    bool IsEmpty() const { return top == 0; }
    void ReturnTo(int pos, bool merge = false);

    int Top() const { return top; }
    int NextIndex() { return top++; }

private:
    SVector values;
    int top;
    
    PlgContext(const PlgContext&);

    int indexValue(const PlgReference &plgIndex) const;
};

class PlgDatabase;
class PlgExpressionContinuation : public SExpression
{
    friend class PlgDatabase;
    friend class PlgExpressionChoicePoint;
    friend class PlgExpressionClauseChoicePoint;
public:
    static IntelibTypeId TypeId;

    PlgExpressionContinuation(PlgDatabase &db, const PlgReference &req);

    bool Next();
    PlgReference GetValue(const PlgReference &var);

    void PushChoicePoint(const PlgReference &point);
    void PopChoicePoint();

    PlgDatabase &Database() { return database; }
    SReference ChoicePoints() { return choicePoints; }
    PlgContext &Context() { return context; }

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
    PlgDatabase() : clauses(*PTheEmptyList) {}

    void Add(const PlgReference &clause);
    PlgContinuation Query(const PlgReference &request);
    SReference Head() const { return clauses; }
private:
    SReference clauses;
    PlgDatabase(const PlgDatabase &other);
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
        : SExpression(typeId), cont(c), contextPos(c.context.Top()), queries(c.queries) {}
};

typedef GenericSReference<PlgExpressionChoicePoint, IntelibX_not_a_prolog_choice_point> PlgChoicePoint;

class PlgExpressionClauseChoicePoint : public PlgExpressionChoicePoint
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionClauseChoicePoint(const PlgReference &cls, PlgExpressionContinuation &c)
        : PlgExpressionChoicePoint(TypeId, c), clause(cls) , pointer(c.Database().Head()) {}

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
        : PlgClauseChoicePoint_Super(new PlgExpressionClauseChoicePoint(clause, cont)) {}
};

#endif
