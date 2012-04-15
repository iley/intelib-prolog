#ifndef INTELIB_PROLOG_PLGCONTEXT_HPP_SENTRY
#define INTELIB_PROLOG_PLGCONTEXT_HPP_SENTRY

#include "../sexpress/svector.hpp"
#include "../sexpress/shashtbl.hpp"

#include "plgdata.hpp"

class PlgContext
{
public:
    PlgContext();

    void Set(const PlgReference &var, const PlgReference &value);
    PlgReference Get(const PlgReference &var) const;

    void ReturnTo(int pos, bool merge = false);
    int NextFrame();

    int Top() const { return top; }
    const SHashTable &ValueTable() const { return values; }

private:
    SHashTable values;
    SVector frameVars;
    int top;

    PlgContext(const PlgContext&);
};

#endif
