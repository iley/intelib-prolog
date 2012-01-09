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
}
