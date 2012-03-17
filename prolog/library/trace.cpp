#include <stdio.h>
#include "../prolog.hpp"
#include "trace.hpp"

namespace PlgStdLib
{
    static void Call(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        fprintf(stderr, "* Call %s\n", PlgTerm(functor, args)->TextRepresentation().c_str());
        fflush(stderr);
    }

    static void Exit(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont, bool result)
    {
        fprintf(stderr, "* %s %s\n", (result ? "Exit" : "Fail"), PlgTerm(functor, args)->TextRepresentation().c_str());
        fflush(stderr);
    }

    bool PredicateTrace(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgGlobalHooks.Call = Call;
        PlgGlobalHooks.Exit = Exit;
        return true;
    }

    PlgAtom trace("trace", 0, PredicateTrace, false);

    bool PredicateNotrace(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        PlgGlobalHooks.Call = 0;
        PlgGlobalHooks.Exit = 0;
        return true;
    }

    PlgAtom notrace("notrace", 0, PredicateNotrace, false);
}
