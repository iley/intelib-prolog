#include <stdio.h>
#include "io.hpp"
#include "../sexpress/shashtbl.hpp"
#include "../tools/sstream.hpp"

namespace PlgStdLib
{
    SHashTable &StreamTable()
    {
        static SHashTable table;
        static bool initialized = false;

        if (!initialized) {
            //TOOD: add std. streams to the table
        }

        return table;
    }

    bool PredicateNl(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        // temporary implementation
        printf("\n");
        return true;
    }

    PlgAtom nl("nl", 0, PredicateNl, false);

    bool PredicatePrint(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
    {
        // temporary implementation
        PlgReference arg = args.Car();
        printf("%s", arg->TextRepresentation().c_str());
        return true;
    }

    PlgAtom print("print", 1, PredicatePrint, false);
    PlgAtom write("write", 1, PredicatePrint, false); //STUB
}
