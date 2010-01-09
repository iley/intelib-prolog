//   InteLib                                    http://www.intelib.org
//   The file ill/ill_loop.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "../sexpress/sexpress.hpp"
#include "../tools/sreader.hpp"
#include "../tools/sstream.hpp"
#include "../genlisp/conteval.hpp"
#include "../genlisp/lispform.hpp"
#include "../lisp/lisp.hpp"
#include "../lisp/lsymbol.hpp"
#include "../lisp/lpackage.hpp"
#include "../lisp/lreader.hpp"
#include "../lisp/llambda.hpp"

#ifndef INTELIB_NILL_TRACE_SUPPORT
#define INTELIB_NILL_TRACE_SUPPORT 1
#endif

#if INTELIB_NILL_TRACE_SUPPORT == 1
#include "illtrace.hpp"
#endif


#include "ill_loop.hpp"

    // defined in ../sexpress/ireadln.cpp
SReference intelib_read_line(const SStreamRef &in,
                             const SStreamRef &out,
                             const SStreamRef &err,
                             IntelibGenericReader &reader,
                             const SReference &package,
                             const SString &prompt);


   // implemented in sexpress/stackrep.cpp
SString stack_representation(SReference stack);

/////////////////////////////////////////////////////////

class LFunctionNooperation : public SExpressionForm {
public:
    LFunctionNooperation() : SExpressionForm() {}
    ~LFunctionNooperation() {}
    virtual SString TextRepresentation() const 
        { return "#<FORM %%%>"; }                       
    virtual void Call(const SReference &, IntelibContinuation &lf) const 
        { lf.RegularReturn(*PTheEmptyList); }            
};

/////////////////////////////////////////////////////////

class LFunctionQuit : public SExpressionForm {
    IntelibLispLoop *the_loop;
public:
    LFunctionQuit(IntelibLispLoop *a_loop) 
        : SExpressionForm(), the_loop(a_loop) {}
    ~LFunctionQuit() {}
    virtual SString TextRepresentation() const 
        { return "#<FORM QUIT>"; }                       
    virtual void Call(const SReference &params, IntelibContinuation &lf) const 
        {
             the_loop->Break(params);
             lf.RegularReturn(params); 
        }            
};

/////////////////////////////////////////////////////////

class LFunctionBody : public SExpressionFunction {
public:
    LFunctionBody() : SExpressionFunction(1, 1) {}
    ~LFunctionBody() {}

    virtual SString TextRepresentation() const 
    { return "#<FUNCTION BODY>"; } 

    virtual void DoApply(int paramsc, const SReference paramsv[], 
                         IntelibContinuation &lf) const
    {
        LExpressionLambda *f =
            paramsv[0].DynamicCastGetPtr<LExpressionLambda>();
        if(f) { 
            lf.ReferenceReturn(f->GetBody(), paramsv[0]);
            return;
        }
        LExpressionMacro *m =
            paramsv[0].DynamicCastGetPtr<LExpressionMacro>();
        if(m) { 
            lf.ReferenceReturn(m->GetBody(), paramsv[0]);
            return;
        }
        lf.RegularReturn(*PTheEmptyList);
    }            
};


/////////////////////////////////////////////////////////

class LFunctionLoad : public SExpressionFunction {
    IntelibLispLoop *the_loop;
public:
    LFunctionLoad(IntelibLispLoop *a_loop) 
        : SExpressionFunction(1, 1), the_loop(a_loop) {}
    ~LFunctionLoad() {}

    virtual SString TextRepresentation() const 
    { return "#<FUNCTION LOAD>"; }                       

    virtual void DoApply(int paramsc, const SReference paramsv[], 
                         IntelibContinuation &lf) const
    {
        const char *fname = paramsv[0].GetString();
        FILE *fl = fopen(fname, "r");
        if(!fl) throw IntelibX("file not found", paramsv[0]);
        SStreamTextInput stream(fl);
        SReference res = the_loop->Load(stream, fname);
        the_loop->Unbreak();
        fclose(fl);
        lf.RegularReturn(res); 
    }            
};


/////////////////////////////////////////////////////////

IntelibLispLoop::IntelibLispLoop(const SReference& a_package) 
    : package(a_package)
{
    LSymbol noop("%%%");
    noop->SetFunction(LReference(new LFunctionNooperation));
    ImportSymbol(noop);

    LSymbol quit("QUIT");
    quit->SetFunction(LReference(new LFunctionQuit(this)));
    ImportSymbol(quit);
    ImportSymbol(quit, "%%%QUIT");

    LSymbol load("LOAD");
    load->SetFunction(LReference(new LFunctionLoad(this)));
    ImportSymbol(load);
    ImportSymbol(load, "%%%LOAD");

    LSymbol body("BODY");
    body->SetFunction(LReference(new LFunctionBody));
    ImportSymbol(body);
    ImportSymbol(body, "%%%BODY");

#if INTELIB_NILL_TRACE_SUPPORT == 1
    LSymbol trace("TRACE");
    trace->SetFunction(LReference(new LFunctionTrace));
    ImportSymbol(trace);
    ImportSymbol(trace, "%%%TRACE");

    LSymbol untrace("UNTRACE");
    untrace->SetFunction(LReference(new LFunctionUntrace));
    ImportSymbol(untrace);
    ImportSymbol(untrace, "%%%UNTRACE");
#endif
}

SReference IntelibLispLoop::Go(IntelibContinuation *a_lf)
{
    SStreamStdin in;
    SStreamStdout out;
    SStreamStderr err;
    return Go(in, out, err, a_lf);
}

bool IntelibLispLoop::ImportSymbol(const LReference& symb, 
                                   const char *name, 
                                   bool safe)
{
    return 
        static_cast<LExpressionPackage*>(package.GetPtr())->
            Import(symb, name, safe);
}

SReference IntelibLispLoop::Go(const SStreamRef &in, 
                               const SStreamRef &out, 
                               const SStreamRef &err, 
                               IntelibContinuation *lf)
{
    break_flag = false;
    exit_code = SReference();

    if(lf) {
        SReference lex_symbols =
            static_cast<LExpressionContext*>(lf->GetContext().GetPtr())
                ->GetAllSymbols();
        while(!lex_symbols.IsEmptyList()) {
            LSymbolRef symb(lex_symbols.Car());
            ImportSymbol(symb);
            lex_symbols = lex_symbols.Cdr();
        }
    }

    LExpressionPackage *pkg = 
        static_cast<LExpressionPackage*>(package.GetPtr());

    static LispReader *local_reader = 0;
    if(!PTheIntelibReader) {
        if(!local_reader) local_reader = new LispReader;
        PTheIntelibReader = local_reader;
    }

    PTheIntelibReader->SetPackage(pkg);

    do {
        LReference r;
        try {
            if(PTheIntelibReader->IsReady())
                r = PTheIntelibReader->Get();
            else
                r = intelib_read_line(in, out, err,
                                      *PTheIntelibReader, package, "nill> ");
        }
        catch(const IntelibX_reader_error &ex) {
            err->Puts("#* reader: ");
            err->Puts(ex.Parameter().GetString());
            err->Puts("\n");
            PTheIntelibReader->Drop();
            continue;
        }
        if(r==IntelibReader::EofMarker) {
            err->Puts("#* EOF. Goodbye\n");
            break;
        }
        try {
            LReference res =
                lf ? r.Evaluate(*static_cast<LispContinuation*>(lf)) 
                   : r.Evaluate();
            out->Puts(res.TextRepresentation().c_str());
            out->Puts("\n");
        }
        catch(const IntelibX_reader_error &ex) {
            SString s("#* Syntax error: ");
            s += ex.Parameter().GetString();
            char buf[40];
            snprintf(buf, sizeof(buf), " line: %d", ex.Line());
            s += buf;
            if(ex.ExpBegLine() > 0 && ex.ExpBegLine() != ex.Line()) {
                snprintf(buf, sizeof(buf), ", expression began at: %d ", 
                                           ex.ExpBegLine());
                s += buf;
            }
            s += "\n";
            err->Puts(s.c_str());            
            PTheIntelibReader->Drop();
        }
        catch(const IntelibX& ex) {
            SString s("#* Exception: ");
            s += ex.Description();
            if(ex.Parameter().GetPtr()) {
                s += " (";
                s += ex.Parameter()->TextRepresentation();
                s += ")";
            }
            s += "\n";
            if(ex.Stack().GetPtr()) {
                s += stack_representation(ex.Stack());
            }
            err->Puts(s.c_str());
            PTheIntelibReader->Drop();
        }
    } while(!break_flag);

    if(local_reader) {
        if(PTheIntelibReader == local_reader)
            PTheIntelibReader = 0;
        delete local_reader;
        local_reader = 0;
    }

    return exit_code;
}

SReference IntelibLispLoop::Load(const SStreamRef &in, const char *fname)
{
    break_flag = false;
    exit_code = SReference();

    LExpressionPackage *pkg = 
        static_cast<LExpressionPackage*>(package.GetPtr());
    PTheIntelibReader->SetPackage(pkg);
    if(fname)
        PTheIntelibReader->SetLine(1, fname);

    do {
        LReference r = PTheIntelibReader->Read(in);
        if(r==IntelibReader::EofMarker) {
            Break(*PTheLispBooleanTrue);
            break;
        }
        try {
            r.Evaluate();
        }
        catch(IntelibX& ex) {
            LListConstructor L;
            ex.AddStack((L|"#* reading line", PTheIntelibReader->GetLine()));
            throw;
        }
    } while(!break_flag);

    return exit_code;
}

void IntelibLispLoop::Break(const SReference &a_exit_code)
{
    break_flag = true;
    exit_code = a_exit_code;
}
