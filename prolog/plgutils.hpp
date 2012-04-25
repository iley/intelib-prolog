#ifndef INTELIB_PROLOG_UTILS_HPP_SENTRY
#define INTELIB_PROLOG_UTILS_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/sstring.hpp"

#include <stdio.h>
#include <stdarg.h>

#include "plgengine.hpp"

//! List length
int Length(const SReference &list);

//! Perl-like string join
SString Join(const SString &delim, const SReference &list);

#if INTELIB_TEXT_REPRESENTATIONS == 1
const char *Dump(const SReference &s);

SString DumpHashTable(const SReference &table);
SString DumpContext(const PlgContext &context);
#endif

#endif
