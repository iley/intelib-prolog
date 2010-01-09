//   InteLib                                    http://www.intelib.org
//   The file ils/nils.cpp
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
#include <string.h>
#include <signal.h>
#include <dlfcn.h>
#include "../sexpress/sexpress.hpp"
#include "../sexpress/sstring.hpp"
#include "../tools/sstream.hpp"
#include "../tools/sreader.hpp"
#include "../scheme/scheme.hpp"
#include "../scheme/schpack.hpp"
#include "../scheme/schsymb.hpp"
#include "../scheme/schfun.hpp"
#include "../scheme/schreadr.hpp"
#include "sch_std.hpp"

#include "ils_loop.hpp"

#include "../version.h"

const char *version = INTELIB_VERSION;


SString stack_representation(SReference stack);


void banner()
{
    printf("\n"
        "  .   .                    .       .          ....      \n"
        "  ..  .                    .       .         .    '     \n"
        "  . . .                    .       .         `....      \n"
        "  .  ..                    .       .              .     \n"
        "i .   . t e r a c t i v e  . n t e .... i b  `....'cheme\n\n\n");
    printf("%s\n%s\n", the_intelib_title, the_intelib_copyright);
    printf("  iNteractive InteLib Scheme interpreter\n");
    printf("  Free software. See GNU GPL v.2 for details\n\n");
}

void help()
{
    banner();
    printf("\nUsage:\n"
        "  nils [-q] [{-i <file> | -x <expression> | -b <module>}] "
        "[<mainfile>|-|-Q] [args]\n"
    );
}

bool load_nils_plugin(IntelibSchemeLoop *the_loop, const char *fname)
{
    void *hdl = dlopen(fname, RTLD_NOW);
    if(!hdl) {
        return false;
    }
    void *sym = dlsym(hdl, "nils_lib_init_function");
    if(!sym) {
        return false;
    }
    void (*fun)(SReference*) = (void(*)(SReference*)) sym;
    SReference r;
    fun(&r);
    for(; !r.IsEmptyList(); r = r.Cdr()) {
        the_loop->ImportSymbol(r.Car());
    }
    return true;
}

/////////////////////////////////////////////////////////

class SchFunctionDload : public SExpressionFunction {
    IntelibSchemeLoop *the_loop;
public:
    SchFunctionDload(IntelibSchemeLoop *a_loop) 
        : SExpressionFunction(1, 1), the_loop(a_loop) {}
    ~SchFunctionDload() {}

    virtual SString TextRepresentation() const 
    { return "#<FUNCTION DLOAD>"; }                       

    virtual void DoApply(int paramsc, const SReference paramsv[], 
                         IntelibContinuation &lf) const
    {
        const char *fname = paramsv[0].GetString();
        bool res = load_nils_plugin(the_loop, fname);
        if(!res) 
            lf.RegularReturn(dlerror());
        else
            lf.RegularReturn(*PTheSchemeBooleanTrue); 
    }            
};


/////////////////////////////////////////////////////////



void add_stdio_symbols(IntelibSchemeLoop &loop)
{
    SchSymbol in("*STANDARD-INPUT*");
    in->SetGlobalValue(SStreamStdin());
    loop.ImportSymbol(in);
    SchSymbol out("*STANDARD-OUTPUT*");
    out->SetGlobalValue(SStreamStdout());
    loop.ImportSymbol(out);
    SchSymbol err("*STANDARD-ERRORS*");
    err->SetGlobalValue(SStreamStderr());
    loop.ImportSymbol(err);

    SchSymbol dload("DLOAD");
    dload->SetGlobalValue(SReference(new SchFunctionDload(&loop)));
    loop.ImportSymbol(dload);
}

int run_file(IntelibSchemeLoop &loop, const char* fname)
{
    FILE *fl;
    bool need_close = true;
    if(fname[0]=='-' && fname[1]==0) {
        fl = stdin;
        need_close = false;
    } else {
        fl = fopen(fname, "r");
        if(!fl) {
            perror(fname);
            return 1;
        }
    }
    SStreamTextInput sf(fl);
    SchReference res;
    int ret = 0;
    try {
        res = loop.Load(sf);
    }
    catch(const IntelibX_reader_error &ex) {
        SString fn = ex.FileName();
        if(fn == "") fn = fname;
        fprintf(stderr, "%s:%d: syntax error (%s)\n",
                        fn.c_str(), ex.Line(), 
                        ex.Parameter().GetString());
        if(ex.ExpBegLine() > 0 && ex.ExpBegLine() != ex.Line()) {
            fprintf(stderr, "%s:%d: the expression begins here\n", 
                        fn.c_str(), ex.ExpBegLine());
        }
        ret = 2;
    }
    catch(const IntelibX& ex) {
        fprintf(stderr, "%s:%d: Exception [%s] (%s) caught "
            "during execution of the file\n",
            fname, sf->TellLine(),
            ex.Description(),
            ex.Parameter().GetPtr() ? 
                ex.Parameter()->TextRepresentation().c_str() : "");
        if(ex.Stack().GetPtr()) {
            fprintf(stderr, "%s:%d: %s",
                fname, sf->TellLine(),
                stack_representation(ex.Stack()).c_str());
        }
        ret = 3;
    }
    if(need_close)
        fclose(fl);
    return ret;
}

void preload_scheme_file(IntelibSchemeLoop &loop, const char* fname)
{
    run_file(loop, fname);
}

void execute_scheme_expressions(IntelibSchemeLoop &loop, const char* expr)
{
    SStreamCharbuf sf(expr);
    try {
        loop.Load(sf);
    }
    catch(const IntelibX_reader_error &ex) {
        fprintf(stderr, "-x:%s: syntax error (%s)\n",
                        expr, 
                        ex.Parameter().GetPtr() ?
                            ex.Parameter().GetString() : "");
    }
    catch(const IntelibX& ex) {
        fprintf(stderr, "-x:%s: Exception [%s] (%s) caught "
                        "during execution\n",
                        expr,
                        ex.Description(),
                        ex.Parameter().GetPtr() ?
                            ex.Parameter()->TextRepresentation().c_str():"");
        if(ex.Stack().GetPtr()) {
            fprintf(stderr, "-x:%s: %s",
                            expr,
                            stack_representation(ex.Stack()).c_str());
        }
    }
}

void sigint_handler(int unused)
{
    signal(SIGINT, sigint_handler);
    SchemeContinuation::InterruptEvaluator();
}

int run_interactive(IntelibSchemeLoop &loop)
{
    signal(SIGINT, sigint_handler);
    for(;;) {
        try {
            loop.Go();
            break;
        }
        catch(SchemeContinuation::Interruption &i) {
            printf("\n#* interrupted *\n");
        }
    }
    return 0;
}

SchExpressionPackage* MakeSchemePackage();

int main(int argc, char *argv[])
{
    SReference package(MakeSchemePackage());
    SchemeReader reader;
    reader.SetPackage(static_cast<SchExpressionPackage*>(package.GetPtr()));
    PTheIntelibReader = &reader;

    IntelibSchemeLoop loop(package);
    add_stdio_symbols(loop);
    bool quiet = false;
    bool quit_after_commands = false;
    int i;
    for(i=1; i<argc;) {
        SString opt(argv[i]);
        if(opt == "-q") {
            i++;
            quiet = true;
            continue;
        } else 
        if(opt == "-i") {
            if(i+1>=argc || argv[i+1][0] == '-') {
                fprintf(stderr, "-i requires an argument\n");
                exit(1);
            }
            preload_scheme_file(loop, argv[i+1]);
            i+=2;
            continue;
        } else 
        if(opt == "-x") {
            if(i+1>=argc || argv[i+1][0] == '-') {
                fprintf(stderr, "-x requires an argument\n");
                exit(1);
            }
            execute_scheme_expressions(loop, argv[i+1]);
            i+=2;
            continue;
        } else 
        if(opt == "-b") {
            if(i+1>=argc || argv[i+1][0] == '-') {
                fprintf(stderr, "-b requires an argument\n");
                exit(1);
            }
            if(!load_nils_plugin(&loop, argv[i+1])) {
                fprintf(stderr, "loading %s: %s\n", argv[i+1], dlerror());
                exit(1);
            }
            i+=2;
            continue;
        } else 
        if(opt == "-Q") {
            i++;
            quit_after_commands = true;
            continue;
        } else 
        if(opt == "-h") {
            help();
            exit(0);
        } else 
        if(opt == "--") {
            /* no more options */
            i++;
            break;
        } else 
        if(argv[i][0]=='-' && argv[i][1]!='\0') {
            fprintf(stderr, "unknown option %s\n", argv[i]);
            exit(1);
        } else
            break;
    }
    /* well... we'll definitely need the *ARGS* symbol */
    SchSymbol lispargs("*ARGS*");
    loop.ImportSymbol(lispargs);
    if(i<argc) {
        /* no interaction... file is given */
        SString lispfile(argv[i]);
        SReference args = *PTheEmptyList;
        for(i=i+1; i<argc; i++) 
            args , argv[i];
        lispargs->SetGlobalValue(args);
        return run_file(loop, lispfile.c_str());
        return 1;
    } else {
        /* No file: run interactively */
        if(!quit_after_commands) {
            if(!quiet)
                banner();
            lispargs->SetGlobalValue(*PTheEmptyList);
            return run_interactive(loop);
        }
    }
    return 0;
}
