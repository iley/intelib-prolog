#ifndef INTELIB_PROLOG_UTILS_HPP_SENTRY
#define INTELIB_PROLOG_UTILS_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/sstring.hpp"

int Length(const SReference &list);
SString Join(const SString &delim, const SReference list);

#endif
