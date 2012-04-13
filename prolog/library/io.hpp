#ifndef INTELIB_PROLOG_IO_HPP_SENTRY
#define INTELIB_PROLOG_IO_HPP_SENTRY

#include "../plgdata.hpp"

namespace PlgStdLib
{
    // ISO streams
    extern PlgAtom current_input;
    extern PlgAtom current_output;
    extern PlgAtom current_error;

    extern PlgAtom user_input;
    extern PlgAtom user_output;
    extern PlgAtom user_error;

    extern PlgAtom open;
    extern PlgAtom close;
    extern PlgAtom flush_output;

    // Low-level I/O
    extern PlgAtom seek;
    extern PlgAtom get;
    extern PlgAtom put;
    extern PlgAtom nl;
    extern PlgAtom tab;

    // Term I/O
    extern PlgAtom read;
    extern PlgAtom write;
    extern PlgAtom print;

    extern PlgAtom format;
}

#endif
