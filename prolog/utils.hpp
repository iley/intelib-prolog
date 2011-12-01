#ifndef INTELIB_PROLOG_UTILS_HPP_SENTRY
#define INTELIB_PROLOG_UTILS_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/sstring.hpp"

//! List length
int Length(const SReference &list);

//! Perl-like string join
SString Join(const SString &delim, const SReference list);

#endif
