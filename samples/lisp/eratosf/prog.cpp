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
#include <stdlib.h>

/* include header files of the InteLib Lisp */
#include <intelib/sexpress/sexpress.hpp>
#include <intelib/sexpress/iexcept.hpp>
#include <intelib/sexpress/sstring.hpp>
#include <intelib/lisp/lisp.hpp>
#include <intelib/lisp/lsymbol.hpp>
#include <intelib/lfun_std.hpp>


#include "prog.hxx"

int main(int argc, char *argv[])
{
    LListConstructor L;
    try {
        printf("Eratosfen's algorythm. InteLib Lisp demo\n"
               "(c) Andrey V. Stolyarov 2000-2007\n"
               "This is free software. See GNU GPL v.2 for details\n\n");
        printf("Trying to initialize the lisp code... ");
        LispInit_prog();
        printf("finished\n");
        LReference Go((L| ERATOSFEN, argc > 1 ? atol(argv[1]) : 100L ));
        LReference res = Go.Evaluate();
        printf("%s\n", res.TextRepresentation().c_str());
        printf("Done.\n");
    }
    catch(IntelibX &x) {
        printf("\nCaught IntelibX: %s\n", x.Description() );
        if(x.Parameter().GetPtr()) {
            printf("%s\n", x.Parameter()->TextRepresentation().c_str());
        }
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    return 0;
}
