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
#include <unistd.h>
#include <string.h>


#include "../sexpress/sexpress.hpp"
#include "../sexpress/iexcept.hpp"
#include "../sexpress/sstring.hpp"
#include "../tools/sstream.hpp"
#include "../lisp/lisp.hpp"
#include "../lisp/lsymbol.hpp"
#include "../lisp/lreader.hpp"

#include "lfun_std.hpp"
#include "lfun_sel.hpp"
#include "lfun_str.hpp"
#include "lfun_io.hpp"
#include "lfun_hsh.hpp"
#include "lfun_rdr.hpp"

static LListConstructor L;

#ifndef HXXFILE
#  define HXXFILE "ill.hxx"
#endif
#include HXXFILE

#include "../version.h"

#ifndef INTELIB_VERSION
#error The symbol INTELIB_VERSION must be defined 
#endif

const char *version = INTELIB_VERSION;

const char defsfilename[] = "illdef._ls";   
const char dirseparator = '/';
const char dirseparatorstr[2] = {dirseparator, 0};
const char illdefsenvvar[] = "ILLDEFSFILE";

void PrintVersion() {
    printf("%s\n%s\n", the_intelib_title, the_intelib_copyright);
    printf("  InteLib Lisp to C++ translator ILL\n");
    printf("  Free software. See GNU GPL v.2 for details\n\n");
}

void PrintHelp() {
    PrintVersion();
    printf("\n"
           "Usage: ill [options] [directives] files\n"
           "    options: -l   don't load the default directives file\n"
           "             -v   be verbose\n"
           "             -V   print version information and exit\n"
           "             -h   display this help and exit\n"
           "    for the list of directives, please refer to documentation\n"
           "\n");
}

SString DefsFilePath(const char *argv0)
{
    // first, check the environment variable
    const char * envpath = getenv(illdefsenvvar);
    if(envpath) return envpath;
  
    // now, try to extract the path from the argv[0]
    const char *last_slash = 0;
    for(const char *p = argv0; *p; p++) 
        if(*p == dirseparator) last_slash = p;
    if(last_slash) { // in this case, just assume the file is there
        int len = last_slash - argv0 + 1 + sizeof(defsfilename);
        char *buf = new char[len];
        int i;
        char *p;
        for(i=0, p=buf; i < last_slash-argv0+1; i++, p++) 
            *p = argv0[i];
        for(i=0; i < (signed)sizeof(defsfilename); i++, p++) 
            *p = defsfilename[i];
        SString ret(buf);
        delete [] buf;
        return ret;
    }
  
    // Well, looks like we were run using PATH variable
    char *pathvar = strdup(getenv("PATH"));
    char *cur = pathvar;
    char *next = pathvar;
    do {
        while(*next && *next != ':') next++;
        if(*next) { *next = 0; next++; }
        SString path(cur);
        path += dirseparatorstr;
        SString binpath = path + argv0;
        if(access(binpath.c_str(), X_OK) == 0) {
            // our binary file found. good. 
            free(pathvar);
            path += defsfilename;
            return path;
        }
        cur = next;
    } while (*cur);
    free(pathvar);
  
    // nothing found... lets just try the current directory
    return defsfilename;
}

bool verbose = false;

LReference ParseCommandLine(int argc, char *argv[])
{
    LReference files = *PTheEmptyList;
    SString options = "";
    bool adddefs = true;
    int i;
    for(i = 1; i < argc; i++) {
        if(argv[i][0] == '-') {
            SString s(argv[i]+1);
            if(s == "l") {
                adddefs = false;
            } else
            if(s == "v") {
                PrintVersion();
                verbose = true;
            } else 
            if(s == "V") {
                PrintVersion();
                exit(0);
            } else 
            if(s == "h") {
                PrintHelp();
                exit(0);
            }
        } else if (argv[i][0] == '(') {    
            options += argv[i];
        } else {
            files.operator,(argv[i]);
        } 
    }
    if(files.IsEmptyList()) {
        return *PTheEmptyList;
    }
    if(adddefs) {
        SString defsfile = DefsFilePath(argv[0]);
        if(verbose) {
            printf("Using the defaults file %s\n", defsfile.c_str());
        }
        files = (L| defsfile || files);
    }
    return (L| options || files);
}

int main(int argc, char *argv[])
{
    LispReader reader;
    PTheIntelibReader = &reader;

    try {
        TheLispProgram();
        SStreamStdout S_stdout;
        LISPSTDOUT->SetDynamicValue(S_stdout);


        LReference commandline = ParseCommandLine(argc, argv);

        if(verbose) {
            printf("%s\n", commandline.TextRepresentation().c_str());
        }
        if(commandline.IsEmptyList()) {
            fprintf(stderr, "No files specified, aborting...\n");
            return 1;
        }
        try {
            LReference Go0((L|HANDLECOMMANDLINEDIRECTIVES,commandline.Car()));
            Go0.Evaluate();    
            LReference Go((L| LISPENTRY, ~LReference(commandline.Cdr())));
            LReference res = Go.Evaluate();
            if(verbose)
                printf("  --> %s\n", res.TextRepresentation().c_str());
        }
        catch(IntelibX_lisp_error_function &ex) {
            SStreamStderr S_stderr;
            LReference err_call(
                (L|ERROR_EXPLANATION, ~ex.Parameter(), ~S_stderr));
            err_call.Evaluate();
            return 1;
        }
        if(verbose)
            printf("  All done, grats. Good bye.\n");
        return 0;
    }
    catch(IntelibX &ex) {
        fprintf(stderr, "Exception: %s ", ex.Description());
        if(ex.Parameter().GetPtr())
            fprintf(stderr, "[%s]", 
                    ex.Parameter()->TextRepresentation().c_str());
        printf("\n");
        if(ex.Stack().GetPtr()) 
            fprintf(stderr, "Stack: <* %s *>\n", 
                    ex.Stack()->TextRepresentation().c_str());
    }
    catch(...) {
        fprintf(stderr, "Unknown exception. Contact developers please\n");
    }
    return 2;
}
