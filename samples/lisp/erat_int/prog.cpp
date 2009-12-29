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
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

/* include header files of the InteLib Lisp */
#include <intelib/sexpress/sexpress.hpp>
#include <intelib/sexpress/iexcept.hpp>
#include <intelib/sexpress/sstring.hpp>
#include <intelib/genlisp/conteval.hpp>
#include <intelib/lisp/lisp.hpp>
#include <intelib/lisp/lsymbol.hpp>
#include <intelib/lfun_std.hpp>


#include "prog.hxx"

void alarm_handle(int)
{
    IntelibContinuation::InterruptEvaluator();
}

int main(int argc, char *argv[])
{
    printf("Eratosfen's algorythm. InteLib Lisp demo\n"
           "(c) Andrey V. Stolyarov 2000-2007\n"
           "This is free software. See GNU GPL v.2 for details\n\n");

    signal(SIGALRM, alarm_handle);
    int allowed_time = argc>2 ? atol(argv[2]) : 5;
    printf("Allowing %d seconds for computations...\n", allowed_time);
    alarm(allowed_time);

    try {
        LListConstructor L;
        printf("Trying to initialize the lisp code... ");
        LispInit_prog();
        printf("finished\n");
        LReference Go((L| ERATOSFEN, argc > 1 ? atol(argv[1]) : 1000L ));
        LReference res = Go.Evaluate();
        printf("%s\n", res.TextRepresentation().c_str());
        printf("Done, without being interrupted.\n");
    }
    catch(IntelibX &x) {
        printf("\nCaught IntelibX: %s\n", x.Description() );
        if(x.Parameter().GetPtr()) {
            printf("%s\n", x.Parameter()->TextRepresentation().c_str());
        }
    }
    catch(IntelibContinuation::Interruption) {
        printf("Evaluation interrupted; the result follows:\n%s\n",
            CURRENT_RESULT->GetDynamicValue()->TextRepresentation().c_str());
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    return 0;
}
