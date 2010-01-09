//   InteLib                                    http://www.intelib.org
//   The file samples/lisp/permut/main.cpp
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
#include <stdlib.h>

#include <intelib/sexpress/sexpress.hpp>
#include <intelib/sexpress/iexcept.hpp>
#include <intelib/sexpress/sstring.hpp>
#include <intelib/tools/sstream.hpp>
#include <intelib/lisp/lisp.hpp>
#include <intelib/lisp/lsymbol.hpp>
#include <intelib/lfun_std.hpp>

#include "ppr.hxx"


int main(int argc, char *argv[])
{
    int n;
    LListConstructor L;
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
        printf("Trying to initialize the lisp code... ");
        LispInit_ppr();
        printf("finished\n");

        SStreamStdout lstdout;

        (L|LISPMAIN, n, lstdout).Evaluate();

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
