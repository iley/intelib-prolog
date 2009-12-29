// +-------------------------------------------------------------------------+
// |               I__n__t__e__L__i__b           0.6.21 development          |
// | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
// |                                                                         |
// | This is free software.  See GNU GENERAL PUBLIC LICENSE v.2 for details. |
// | GNU GPL v2  is  found  in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
// |     Please see also docs/readme.txt and visit http://www.intelib.org    |
// |                                                                         |
// | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
// +-------------------------------------------------------------------------+




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


