#include "utils.hpp"

int Length(const SReference &list) {
    int len = 0;
    for (SReference p = list; !p.IsEmptyList(); p = p.Cdr()) {
        ++len;
    }
    return len;
}

SString Join(const SString &delim, const SReference list) {
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
