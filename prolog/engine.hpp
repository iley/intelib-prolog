#ifndef INTELIB_PROLOG_ENGINE_HPP_SENTRY
#define INTELIB_PROLOG_ENGINE_HPP_SENTRY

#include "data.hpp"
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

protected:
    PlgContext::Frame *frame;
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
    SQueue clauses;
    PlgDatabase(const PlgDatabase &other);
};

#endif
