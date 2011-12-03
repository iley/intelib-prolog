#ifndef INTELIB_PROLOG_ENGINE_HPP_SENTRY
#define INTELIB_PROLOG_ENGINE_HPP_SENTRY

#include "data.hpp"
#include "../sexpress/sexpress.hpp"
#include "../sexpress/gensref.hpp"
#include "../sexpress/shashtbl.hpp"

class PlgContext
{
public:
    class Frame {
        friend class PlgContext;
    public:
        Frame(Frame *p) : prev(p) {}

        void Set(const PlgReference &name, const PlgReference &value);
        PlgReference Get(const PlgReference &name) const;

        Frame *Prev() const { return prev; }
        void Apply(const Frame &droppedFrame);
        PlgReference Evaluate(const PlgReference& value) const;
        const SHashTable& Table() const { return table; }

    private:
        SHashTable table;
        Frame *prev;
    };

    PlgContext() : top(0) {}
    ~PlgContext() { Clean(); }

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
    Frame *CurrentFrame();

    void ReturnTo(Frame *frame, bool keepValues = false);
    void DropFrame(bool keepValues = false);
    bool MergeDownFrame();

    void Clean() { ReturnTo(0, false); }

private:
    Frame *top;
};

class PlgDatabase;
class PlgExpressionContinuation : public SExpression
{
public:
    static IntelibTypeId TypeId;

    PlgExpressionContinuation(const PlgDatabase &db, const PlgReference &request);

    bool Next();
    PlgReference GetValue(const PlgReference &var);

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

private:
    const PlgDatabase &database;
    SReference choicePoints;
    PlgContext context;
};

typedef GenericSReference<PlgExpressionContinuation, IntelibX_not_a_prolog_continuation> PlgContinuation_Super;

class PlgContinuation : public PlgContinuation_Super
{
public:
    PlgContinuation(const PlgDatabase &db, const PlgReference &request) : PlgContinuation_Super(new PlgExpressionContinuation(db, request)) {}
};

class PlgChoicePoint : public PlgExpression
{
public:
    virtual bool Next(PlgExpressionContinuation &continuation) = 0;

protected:
    PlgContext::Frame *frame;
};

class PlgClauseChoicePoint : public PlgChoicePoint
{
public:
    PlgClauseChoicePoint(const PlgReference &cl, const SReference &ptr) : clause(cl), pointer(ptr) {}
    virtual bool Next(PlgContinuation &continuation);
private:
    PlgReference clause;
    SReference pointer;
};

class PlgDatabase
{
public:
    PlgDatabase() : clauses(*PTheEmptyList) {}

    void Add(const PlgReference &clause);
    PlgContinuation Query(const PlgReference &request);
private:
    SQueue clauses;
    PlgDatabase(const PlgDatabase &other);
};

#endif
