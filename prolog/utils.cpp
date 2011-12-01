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
        result = result + SString(p);
        if (!p.Cdr().IsEmptyList())
            result = result + delim;
    }
    return result;
}
