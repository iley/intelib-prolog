#ifndef INTELIB_PROLOG_UTILS_HPP_SENTRY
#define INTELIB_PROLOG_UTILS_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/sstring.hpp"
#include <stdio.h>
#include <stdarg.h>

//! List length
int Length(const SReference &list);

//! Perl-like string join
SString Join(const SString &delim, const SReference list);

/*
class Log
{
public:
    Log(const char *filename) {
        file = fopen(filename, "w");
    }

    ~Log() {
        if (file) fclose(file);
    }

    void Write(const char *format, ...) {
        if (file) {
            va_list args;
            va_start(args,format);
            vfprintf(file, format, args );
            fprintf(file, "\n");
            fflush(file);
        }
    }

private:
    FILE *file;
};

template <class T>
const char *SStr(T &s) { return s->TextRepresentation().c_str(); }
*/

#endif
