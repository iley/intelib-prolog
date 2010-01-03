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

#include <intelib/sexpress/sexpress.hpp>
#include <intelib/sexpress/iexcept.hpp>
#include <intelib/sexpress/sstring.hpp>
#include <intelib/tools/sstream.hpp>
#include <intelib/scheme/scheme.hpp>
#include <intelib/scheme/schsymb.hpp>
#include <intelib/sch_std.hpp>

#include "perm.hpp"
#include "ppr.hxx"


SchSymbol CREATE_PG("CREATE_PG");
SchSymbol NEXT_PERMUTATION("NEXT_PERMUTATION");

int main(int argc, char *argv[])
{
    CREATE_PG->SetGlobalValue(create_permutation_generator);
    NEXT_PERMUTATION->SetGlobalValue(next_permutation);
    int n;
    SchListConstructor L;
    if(argc>1) {
        char *err;
        n = strtol(argv[1], &err, 0);
        if(*err) {
            fprintf(stderr, "Not a number: %s\n", argv[1]);
            exit(1);
        }
    } else {
        n = 5;
    }
    try {
        printf("Trying to initialize the Scheme code... ");
        SchemeInit_ppr();
        printf("finished\n");

        SStreamStdout lstdout;

        (L| SCHEMEMAIN, n, lstdout).Evaluate();

        printf("Done.\n");
    }
    catch(IntelibX &x) {
        printf("\nCaught IntelibX: %s\n", x.Description() );
        if(x.Parameter().GetPtr()) {
            printf("%s\n", x.Parameter()->TextRepresentation().c_str());
        }
        if(x.Stack().GetPtr()) {
            printf("%s\n", x.Stack()->TextRepresentation().c_str());
        }
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    return 0;
}