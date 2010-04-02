//   InteLib                                    http://www.intelib.org
//   The file interact/ill_loop.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2010
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



#include "repl.hpp"

    // defined in ireadln.cpp
SReference intelib_read_line(const SStreamRef &in,
                             const SStreamRef &out,
                             const SStreamRef &err,
                             IntelibGenericReader &reader,
                             const SReference &package,
                             const SString &prompt);


   // implemented in tools/stackrep.cpp
SString stack_representation(SReference stack);

/////////////////////////////////////////////////////////

class SFunctionNooperation : public SExpressionForm {
public:
    SFunctionNooperation() : SExpressionForm() {}
    ~SFunctionNooperation() {}
    virtual SString TextRepresentation() const 
    { return "#<FORM %%%>"; }                       
    virtual void Call(const SReference &, IntelibContinuation &lf) const 
    { lf.RegularReturn(*PTheEmptyList); }            
};

/////////////////////////////////////////////////////////

class SFunctionQuit : public SExpressionForm {
    IntelibRepl *the_loop;
public:
    SFunctionQuit(IntelibRepl *a_loop) 
        : SExpressionForm(), the_loop(a_loop) {}
    ~SFunctionQuit() {}
    virtual SString TextRepresentation() const 
    { return "#<FORM QUIT>"; }                       
    virtual void Call(const SReference &params, IntelibContinuation &lf) const 
    {
         the_loop->Break(params);
         lf.RegularReturn(params); 
    }            
};


/////////////////////////////////////////////////////////

class SFunctionLoad : public SExpressionFunction {
    IntelibRepl *the_loop;
public:
    SFunctionLoad(IntelibRepl *a_loop) 
        : SExpressionFunction(1, 1), the_loop(a_loop) {}
    ~SFunctionLoad() {}

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

IntelibRepl::IntelibRepl(const SReference& a_package) 
    : package(a_package)
{
    LSymbol noop("%%%");
    noop->SetFunction(SReference(new SFunctionNooperation));
    ImportSymbol(noop);

    LSymbol quit("QUIT");
    quit->SetFunction(SReference(new SFunctionQuit(this)));
    ImportSymbol(quit);
    ImportSymbol(quit, "%%%QUIT");

    LSymbol load("LOAD");
    load->SetFunction(SReference(new SFunctionLoad(this)));
    ImportSymbol(load);
    ImportSymbol(load, "%%%LOAD");

#if 0
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
#endif // 0
}

SReference IntelibRepl::Go(IntelibContinuation *a_lf)
{
    SStreamStdin in;
    SStreamStdout out;
    SStreamStderr err;
    return Go(in, out, err, a_lf);
}

#if 0
bool IntelibRepl::ImportSymbol(const LReference& symb, 
                                   const char *name, 
                                   bool safe)
{
    return 
        static_cast<LExpressionPackage*>(package.GetPtr())->
            Import(symb, name, safe);
}
#endif

SReference IntelibRepl::Go(const SStreamRef &in, 
                           const SStreamRef &out, 
                           const SStreamRef &err, 
                           IntelibContinuation *lf)
{
    break_flag = false;
    exit_code = SReference();

    if(lf) {
        ImportLexicalSymbols(lf);
    }

    SExpressionHashPackage *pkg = 
        static_cast<SExpressionHashPackage*>(package.GetPtr());

#error  local reader move off here
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
#error virtualize Continuation preparation
            IntelibContinuation *cont = lf ? lf : new LispContinuation;
            try {
                int mark = cont->GetMark();
                cont->PushTodo(LispContinuation::just_evaluate, r);
                while(
                    !break_flag &&
                    (!extra_break_flag || !*extra_break_flag) &&
                    !cont->Ready(mark)
                )
                    cont->Step();
                if(cont->Ready(mark)) {
                    LReference res = cont->Get();
#error virtualize printing as it is different 
                    out->Puts(res.TextRepresentation().c_str());
                    out->Puts("\n");
                }
            }
            catch(...) {
                if(!lf) delete cont;
                throw;
            }
            if(!lf) delete cont;
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
    } while(!break_flag && (extra_break_flag ? !*extra_break_flag : true));

#error  local reader move off here
    if(local_reader) {
        if(PTheIntelibReader == local_reader)
            PTheIntelibReader = 0;
        delete local_reader;
        local_reader = 0;
    }

    return exit_code;
}

SReference IntelibRepl::Load(const SStreamRef &in, const char *fname)
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
            //r.Evaluate();
            LispContinuation cont;
            int mark = cont.GetMark();
            cont.PushTodo(LispContinuation::just_evaluate, r);
            while(
                !break_flag &&
                (!extra_break_flag || !*extra_break_flag) &&
                !cont.Ready(mark)
            )
                cont.Step();
        }
        catch(IntelibX& ex) {
            LListConstructor L;
            ex.AddStack((L|"#* reading line", PTheIntelibReader->GetLine()));
            throw;
        }
    } while(!break_flag && (extra_break_flag ? !*extra_break_flag : true));

    return exit_code;
}

void IntelibRepl::Break(const SReference &a_exit_code)
{
    break_flag = true;
    exit_code = a_exit_code;
}
