#ifndef INTELIB_PROLOG_ENGINE_HPP_SENTRY
#define INTELIB_PROLOG_ENGINE_HPP_SENTRY

#include "data.hpp"
#include "../sexpress/shashtbl.hpp"

class PlgContext
{
public:
    class Frame {
    public:
        void Set(const PlgVariableName &name, const PlgReference &value);
        PlgReference Get(const PlgVariableName &name);

    private:
        SHashTable table;
    };

    PlgContext() : top(0) {}

    void Set(const PlgVariableName &name, const PlgReference &value);
    PlgReference Get(const PlgVariableName &name);

    Frame *CreateFrame();
    Frame *CurrentFrame();

    void ReturnTo(Frame *frame, bool keepValues = false);
    void DropFrame(bool keepValues = false);

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
    SReference clauses;
    PlgDatabase(const PlgDatabase &other);
};

#endif
