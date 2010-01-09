//   InteLib                                    http://www.intelib.org
//   The file ils/ilsmain.cpp
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
#include <unistd.h>
#include <string.h>


#include "../sexpress/sexpress.hpp"
#include "../sexpress/iexcept.hpp"
#include "../sexpress/sstring.hpp"
#include "../tools/sstream.hpp"
#include "../scheme/scheme.hpp"
#include "../scheme/schsymb.hpp"
#include "../scheme/schfun.hpp"
#include "../scheme/schreadr.hpp"


static SchListConstructor L;

#ifndef HXXFILE
#  define HXXFILE "ils.hxx"
#endif
#include HXXFILE

#include "../version.h"

#ifndef INTELIB_VERSION
#error The symbol INTELIB_VERSION must be defined 
#endif

const char *version = INTELIB_VERSION;

const char defsfilename[] = "ilsdef._sc";   
const char dirseparator = '/';
const char dirseparatorstr[2] = {dirseparator, 0};
const char illdefsenvvar[] = "ILLDEFSFILE";

void PrintVersion() {
    printf("%s\n%s\n", the_intelib_title, the_intelib_copyright);
    printf("  InteLib Scheme to C++ translator ILS\n");
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

SReference ParseCommandLine(int argc, char *argv[])
{
    SReference files = *PTheEmptyList;
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
    SchemeReader reader;
    PTheIntelibReader = &reader;

    try {
        TheSchemeProgram();

        SReference commandline = ParseCommandLine(argc, argv);

        if(verbose) {
            printf("%s\n", commandline->TextRepresentation().c_str());
        }
        if(commandline.IsEmptyList()) {
            fprintf(stderr, "No files specified, aborting...\n");
            return 1;
        }
#if 0
        try {
#endif
            SchReference Go0((L|CMDLINEDIRECTIVES,commandline.Car()));
            Go0.Evaluate();    
            SchReference Go((L|ILS_MAIN, ~SReference(commandline.Cdr())));
            SchReference res = Go.Evaluate();
            if(verbose)
                printf("  --> %s\n", res.TextRepresentation().c_str());
#if 0
        }
        catch(IntelibX_lisp_error_function &ex) {
            SStreamStderr S_stderr;
            SReference err_call(
                (L|ERROR_EXPLANATION, ~ex.Parameter(), ~S_stderr));
            err_call.Evaluate();
            return 1;
        }
#endif
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
