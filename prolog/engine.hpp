#ifndef INTELIB_PROLOG_ENGINE_HPP_SENTRY
#define INTELIB_PROLOG_ENGINE_HPP_SENTRY

#include "data.hpp"
#include "../sexpress/sexpress.hpp"
#include "../sexpress/squeue.hpp"
#include "../sexpress/gensref.hpp"
#include "../sexpress/shashtbl.hpp"

class PlgContext
{
public:
    class Frame {
        friend class PlgContext;
    public:
        Frame(Frame *p, Frame *n=0) : prev(p), next(n) {}

        void Set(const PlgReference &name, const PlgReference &value);
        PlgReference Get(const PlgReference &name) const;

        Frame *Prev() const { return prev; }

        void Apply(const Frame &droppedFrame);
        PlgReference Evaluate(const PlgReference& value) const;
        const SHashTable& Table() const { return table; }

    private:
        SHashTable table;
        Frame *prev, *next;
    };

    PlgContext() : top(0), bottom(0) { CreateFrame(); bottom = top; }
    ~PlgContext();

    void Set(const PlgReference &name, const PlgReference &value) {
        INTELIB_ASSERT(top, IntelibX_unexpected_unbound_value());
        top->Set(name, value);
    }

    PlgReference Get(const PlgReference &name) const {
        INTELIB_ASSERT(top, IntelibX_unexpected_unbound_value());
        return top->Get(name);
    }

    PlgReference Evaluate(const PlgReference &value) const {
        INTELIB_ASSERT(top, IntelibX_unexpected_unbound_value());
        return top->Evaluate(value);
    }

    Frame *CreateFrame();
    Frame *CurrentFrame() const { return top; }

    void ReturnTo(Frame *frame, bool keepValues = false);
    void DropFrame(bool keepValues = false);
    bool MergeDownFrame();

private:
    Frame *top, *bottom;
};

class PlgDatabase;
class PlgExpressionContinuation : public PlgExpression
{
    friend class PlgDatabase;
public:
    static IntelibTypeId TypeId;

    PlgExpressionContinuation(PlgDatabase &db, const PlgReference &req);

    bool Next();
    PlgReference GetValue(const PlgReference &var);

    void PushChoicePoint(const PlgReference &point);

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
    PlgReference request;
};

typedef GenericSReference<PlgExpressionContinuation, IntelibX_not_a_prolog_continuation> PlgContinuation_Super;

class PlgContinuation : public PlgContinuation_Super
{
public:
    PlgContinuation(PlgDatabase &db, const PlgReference &request) : PlgContinuation_Super(new PlgExpressionContinuation(db, request)) {}
};

class PlgExpressionChoicePoint : public PlgExpression
{
public:
    static IntelibTypeId TypeId;

    virtual bool Next(PlgExpressionContinuation &continuation) = 0;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

protected:
    PlgContext::Frame *frame;

    PlgExpressionChoicePoint(const IntelibTypeId &typeId = TypeId) : PlgExpression(typeId) {}
};

typedef GenericSReference<PlgExpressionChoicePoint, IntelibX_not_a_prolog_choice_point> PlgChoicePoint;

class PlgExpressionClauseChoicePoint : public PlgExpressionChoicePoint
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionClauseChoicePoint(const PlgReference &cl, const SReference &ptr, PlgContext::Frame *frm)
        : PlgExpressionChoicePoint(TypeId), clause(cl), pointer(ptr) { frame = frm; }

    virtual bool Next(PlgExpressionContinuation &continuation);
private:
    PlgReference clause;
    SReference pointer;
};

typedef GenericSReference<PlgExpressionClauseChoicePoint, IntelibX_not_a_prolog_clause_choice_point> PlgClauseChoicePoint_Super;

class PlgClauseChoicePoint : public PlgClauseChoicePoint_Super
{
public:
    PlgClauseChoicePoint(const PlgReference &request, const SReference &ptr, PlgContext::Frame *frm)
        : PlgClauseChoicePoint_Super(new PlgExpressionClauseChoicePoint(request, ptr, frm)) {}
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
