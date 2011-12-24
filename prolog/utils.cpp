#include "utils.hpp"
#include "../sexpress/shashtbl.hpp"

int Length(const SReference &list) {
    int len = 0;
    for (SReference p = list; !p.IsEmptyList(); p = p.Cdr()) {
        ++len;
    }
    return len;
}

SString Join(const SString &delim, const SReference &list) {
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

const char *Dump(const SReference &s) {
    if (s->TermType() == SExpressionHashTable::TypeId)
        return DumpHashTable(s).c_str();
    return s->TextRepresentation().c_str();
}

SString DumpHashTable(const SReference &table) {
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

SString DumpContext(const PlgContext &context) {
    int count = 0;
    SString result = "";
    for (PlgContext::Frame *frame = context.Top(); frame != context.Bottom(); frame = frame->Prev()) {
        ++count;
        result += DumpHashTable(frame->Table()) + "\n";
    }
    return result;
}

#endif
