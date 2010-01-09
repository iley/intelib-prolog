//   InteLib                                    http://www.intelib.org
//   The file ill/nill.cpp
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
#include "../lisp/lsymbol.hpp"
#include "../lisp/llambda.hpp"
#include "../lisp/lpackage.hpp"
#include "../lisp/lreader.hpp"

#include "ill_loop.hpp"

#include "../version.h"

   // this function is implemented by the _generated_ file sympkg.cxx
extern LExpressionPackage* MakeLispPackage();

const char *version = INTELIB_VERSION;

void banner()
{
    printf("\n"
        "  .   .                    .       .         .\n"
        "  ..  .                    .       .         .\n"
        "  . . .                    .       .         .\n"
        "  .  ..                    .       .         .\n"
        "i .   . t e r a c t i v e  . n t e .... i b  .... i s p\n\n\n");
    printf("%s\n%s\n", the_intelib_title, the_intelib_copyright);
    printf("  iNteractive InteLib Lisp interpreter\n");
    printf("  Free software. See GNU GPL v.2 for details\n\n");
}

void help()
{
    banner();
    printf("\nUsage:\n"
        "  nill [-q] [{-i <file> | -x <expression> | -b <module>}] "
        "[<mainfile>|-] [args]\n"
    );
}

bool load_nill_plugin(IntelibLispLoop *the_loop, const char *fname)
{
    void *hdl = dlopen(fname, RTLD_NOW);
    if(!hdl) {
        return false;
    }
    void *sym = dlsym(hdl, "nill_lib_init_function");
    if(!sym) {
        return false;
    }
    void (*fun)(LReference*) = (void(*)(LReference*)) sym;
    LReference r;
    fun(&r);
    for(; !r.IsEmptyList(); r = r.Cdr()) {
        the_loop->ImportSymbol(r.Car());
    }
    return true;
}

/////////////////////////////////////////////////////////

class LFunctionDload : public SExpressionFunction {
    IntelibLispLoop *the_loop;
public:
    LFunctionDload(IntelibLispLoop *a_loop) 
        : SExpressionFunction(1, 1), the_loop(a_loop) {}
    ~LFunctionDload() {}

    virtual SString TextRepresentation() const 
    { return "#<FUNCTION DLOAD>"; }                       

    virtual void DoApply(int paramsc, const SReference paramsv[], 
                         IntelibContinuation &lf) const
    {
        const char *fname = paramsv[0].GetString();
        bool res = load_nill_plugin(the_loop, fname);
        if(!res) 
            lf.RegularReturn(dlerror());
        else
            lf.RegularReturn(*PTheLispBooleanTrue); 
    }            
};


/////////////////////////////////////////////////////////




void add_stdio_symbols(IntelibLispLoop &loop)
{
    LSymbol in("*STANDARD-INPUT*");
    in->SetDynamicBinding(true);
    in->SetDynamicValue(SStreamStdin());
    in->SetConstant(true);
    loop.ImportSymbol(in);
    LSymbol out("*STANDARD-OUTPUT*");
    out->SetDynamicBinding(true);
    out->SetDynamicValue(SStreamStdout());
    out->SetConstant(true);
    loop.ImportSymbol(out);
    LSymbol err("*STANDARD-ERRORS*");
    err->SetDynamicBinding(true);
    err->SetDynamicValue(SStreamStderr());
    err->SetConstant(true);
    loop.ImportSymbol(err);

    LSymbol dload("DLOAD");
    dload->SetFunction(LReference(new LFunctionDload(&loop)));
    loop.ImportSymbol(dload);
}

int run_file(IntelibLispLoop &loop, const char* fname)
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
    LReference res;
    int ret = 0;
    try {
        res = loop.Load(sf);
    }
    catch(const IntelibX_reader_error &ex) {
        SString fn = ex.FileName();
        if(fn == "") fn = fname;
        fprintf(stderr, "%s:%d: syntax error (%s)\n",
                        fn.c_str(), ex.Line(), 
                        ex.Parameter().GetPtr() ?
                            ex.Parameter().GetString() : "");
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

void preload_lisp_file(IntelibLispLoop &loop, const char* fname)
{
    run_file(loop, fname);
}

void execute_lisp_expressions(IntelibLispLoop &loop, const char* expr)
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
                        ex.Parameter()->TextRepresentation().c_str() : "");
    }
}

void sigint_handler(int unused)
{
    signal(SIGINT, sigint_handler);
    IntelibContinuation::InterruptEvaluator();
}

int run_interactive(IntelibLispLoop &loop)
{
    signal(SIGINT, sigint_handler);
    for(;;) {
        try {
            loop.Go();
            break;
        }
        catch(IntelibContinuation::Interruption &i) {
            printf("\n#* interrupted *\n");
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    SReference package(MakeLispPackage());
    LispReader reader;
    reader.SetPackage(static_cast<LExpressionPackage*>(package.GetPtr()));
    PTheIntelibReader = &reader;

    IntelibLispLoop loop(package);
    add_stdio_symbols(loop);
    bool quiet = false;
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
            preload_lisp_file(loop, argv[i+1]);
            i+=2;
            continue;
        } else 
        if(opt == "-x") {
            if(i+1>=argc || argv[i+1][0] == '-') {
                fprintf(stderr, "-x requires an argument\n");
                exit(1);
            }
            execute_lisp_expressions(loop, argv[i+1]);
            i+=2;
            continue;
        } else 
        if(opt == "-b") {
            if(i+1>=argc || argv[i+1][0] == '-') {
                fprintf(stderr, "-b requires an argument\n");
                exit(1);
            }
            if(!load_nill_plugin(&loop, argv[i+1])) {
                fprintf(stderr, "loading %s: %s\n", argv[i+1], dlerror());
                exit(1);
            }
            i+=2;
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
    LSymbol lispargs("*ARGS*");
    lispargs->SetDynamicBinding(true);
    loop.ImportSymbol(lispargs);
    if(i<argc) {
        /* no interaction... file is given */
        SString lispfile(argv[i]);
        SReference args = *PTheEmptyList;
        for(i=i+1; i<argc; i++) 
            args , argv[i];
        lispargs->SetDynamicValue(args);
        return run_file(loop, lispfile.c_str());
    } else {
        /* No file: run interactively */
        if(!quiet)
            banner();
        lispargs->SetDynamicValue(*PTheEmptyList);
        return run_interactive(loop);
    }
    return 0;
}
