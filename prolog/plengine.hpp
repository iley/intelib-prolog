#ifndef INTELIB_PROLOG_ENGINE_HPP_SENTRY
#define INTELIB_PROLOG_ENGINE_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/squeue.hpp"
#include "../sexpress/svector.hpp"
#include "../sexpress/gensref.hpp"
#include "../sexpress/shashtbl.hpp"

#include "pldata.hpp"

void Assert(const PlgReference &clause);
void AssertA(const PlgReference &clause);
void AssertWithoutExpansion(const PlgReference &clause);

class PlgContext
{
public:
    PlgContext() : values(), top(0)
    {
        frameVars[0] = *(GetEmptyList());
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
        frameVars[top] = *(GetEmptyList());
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

class PlgExpressionContinuation : public SExpression
{
    friend class PlgExpressionChoicePoint;
    friend class PlgExpressionClauseChoicePoint;
    friend class PlgExpressionDisjChoicePoint;
public:
    static IntelibTypeId TypeId;

    PlgExpressionContinuation(const PlgReference &req);

    bool Next();
    PlgReference GetValue(const PlgReference &var);

    void PushChoicePoint(const PlgReference &point);
    void PopChoicePoint();
    void ResetChoicePoints();
    void Cut();

    SReference ChoicePoints() { return choicePoints; }
    PlgContext &Context() { return context; }

    void PushQuery(const PlgReference &query);

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif


private:
    SReference choicePoints;
    PlgContext context;
    SReference queries;

    bool Backtrack();
};

typedef GenericSReference<PlgExpressionContinuation, IntelibX_not_a_prolog_continuation> PlgContinuation_Super;

class PlgContinuation : public PlgContinuation_Super
{
public:
    PlgContinuation(const PlgReference &request) : PlgContinuation_Super(new PlgExpressionContinuation(request)) {}
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

    PlgExpressionClauseChoicePoint(const PlgReference &cls, PlgExpressionContinuation &c)
        : PlgExpressionChoicePoint(TypeId, c), clause(cls), pointer(cls.Functor()->ProcedureList()) {}

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
