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
    PlgContext() : values(), marks(*PTheEmptyList), top(0) {}

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
    SReference marks; //mark stack
    int top;
    
    PlgContext(const PlgContext&);

    int indexValue(const PlgReference &plgIndex) const;
};

class PlgDatabase;
class PlgExpressionContinuation : public SExpression
{
    friend class PlgDatabase;
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
    PlgReference request;
};

typedef GenericSReference<PlgExpressionContinuation, IntelibX_not_a_prolog_continuation> PlgContinuation_Super;

class PlgContinuation : public PlgContinuation_Super
{
public:
    PlgContinuation(PlgDatabase &db, const PlgReference &request) : PlgContinuation_Super(new PlgExpressionContinuation(db, request)) {}
};

class PlgExpressionChoicePoint : public SExpression
{
public:
    static IntelibTypeId TypeId;

    virtual bool Next(PlgExpressionContinuation &continuation) = 0;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

protected:
    int contextPos;

    PlgExpressionChoicePoint(const IntelibTypeId &typeId = TypeId, int pos = 0) : SExpression(typeId), contextPos(pos) {}
};

typedef GenericSReference<PlgExpressionChoicePoint, IntelibX_not_a_prolog_choice_point> PlgChoicePoint;

class PlgExpressionClauseChoicePoint : public PlgExpressionChoicePoint
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionClauseChoicePoint(const PlgReference &cl, const SReference &ptr, int pos)
        : PlgExpressionChoicePoint(TypeId, pos), clause(cl), pointer(ptr) {}

    virtual bool Next(PlgExpressionContinuation &continuation);
private:
    PlgReference clause;
    SReference pointer;
};

typedef GenericSReference<PlgExpressionClauseChoicePoint, IntelibX_not_a_prolog_clause_choice_point> PlgClauseChoicePoint_Super;

class PlgClauseChoicePoint : public PlgClauseChoicePoint_Super
{
public:
    PlgClauseChoicePoint(const PlgReference &request, const SReference &ptr, int pos)
        : PlgClauseChoicePoint_Super(new PlgExpressionClauseChoicePoint(request, ptr, pos)) {}
};

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

#endif
