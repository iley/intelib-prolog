#include "plgutils.hpp"
#include "../sexpress/shashtbl.hpp"

int Length(const SReference &list)
{
    int len = 0;
    for (SReference p = list; !p.IsEmptyList(); p = p.Cdr()) {
        ++len;
    }
    return len;
}

SString Join(const SString &delim, const SReference &list)
{
    SString result;
    for (SReference p = list; !p.IsEmptyList(); p = p.Cdr()) {
#if INTELIB_TEXT_REPRESENTATIONS == 1
        result = result + p.Car()->TextRepresentation();
#else
        result = result + SString(p);
#endif
        if (!p.Cdr().IsEmptyList())
            result = result + delim;
    }
    return result;
}

#if INTELIB_TEXT_REPRESENTATIONS == 1

const char *Dump(const SReference &s)
{
    static SString str;

    if (!s.GetPtr())
        return "undef";
    else if (s->TermType() == SExpressionHashTable::TypeId)
        str = DumpHashTable(s);
    else
        str = s->TextRepresentation();

    return str.c_str();
}

SString DumpHashTable(const SReference &table)
{
    SExpressionHashTable *tbl = table.DynamicCastGetPtr<SExpressionHashTable>();
    INTELIB_ASSERT(tbl, IntelibX_bug());
    SExpressionHashTable::Iterator it(*tbl);
    SString result = "{ ";

    SReference cons = it.GetNext();
    while (cons.GetPtr()) {
        result += cons.Car()->TextRepresentation() + " => " + cons.Cdr()->TextRepresentation() + ", ";

        cons = it.GetNext();
    }
    return result + "}";
}

SString DumpContext(const PlgContext &context)
{
    return DumpHashTable(context.ValueTable());
}

#endif
