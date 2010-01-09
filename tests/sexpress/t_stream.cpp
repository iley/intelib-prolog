//   InteLib                                    http://www.intelib.org
//   The file tests/sexpress/t_stream.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include <stdio.h>
#include "tests.hpp"

#include "sexpress/iexcept.hpp"
#include "sexpress/sstring.hpp"
#include "tools/sstream.hpp"


void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}

int main()
{
    poc();
    try {
        TestSection("Intelib Streams");
        TestSubsection("StreamCharBuf_Gets");
        {
            SStreamCharbuf cb("first\nsecond\nthird");
            char buf[20];
            TESTB("gets_ok", cb->Gets(buf, sizeof(buf)) == buf);
            TEST("gets", buf, "first\n");
            TESTB("gets_short_ok", cb->Gets(buf, 4) == buf);
            TEST("gets_short", buf, "sec");
            TESTB("gets2_ok", cb->Gets(buf, sizeof(buf)) == buf);
            TEST("gets2", buf, "ond\n");
            TESTB("gets_at_eof_ok", cb->Gets(buf, sizeof(buf)) == buf);
            TEST("gets_at_eof", buf, "third");
            TESTB("gets_eof", cb->Gets(buf, sizeof(buf)) == 0);
        }
        TestScore();
    }
    catch(const IntelibX &ex) {
        printf("Caught IntelibX: %s\n%s\n", ex.Description(),
                ex.Parameter()->TextRepresentation().c_str());
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    poc();
    return 0;
}


