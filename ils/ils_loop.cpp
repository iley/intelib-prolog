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




#include "../sexpress/sexpress.hpp"
#include "../tools/sreader.hpp"
#include "../tools/sstream.hpp"
#include "../scheme/scheme.hpp"
#include "../scheme/schpack.hpp"
#include "../scheme/schfun.hpp"
#include "../scheme/schsymb.hpp"

#if 0
#ifndef INTELIB_NILL_TRACE_SUPPORT
#define INTELIB_NILL_TRACE_SUPPORT 1
#endif
#if INTELIB_NILL_TRACE_SUPPORT == 1
#include "illtrace.hpp"
#endif
#endif


#include "ils_loop.hpp"


extern
SReference intelib_read_line(const SStreamRef &in,
                             const SStreamRef &out,
                             const SStreamRef &err,
                             IntelibGenericReader &reader,
                             const SReference &package,
                             const SString &prompt);

SString stack_representation(SReference stack);


/////////////////////////////////////////////////////////

class SchFunctionNooperation : public SExpressionForm {
public:
    SchFunctionNooperation() : SExpressionForm() {}
    ~SchFunctionNooperation() {}
    virtual SString TextRepresentation() const 
        { return "#<FORM %%%>"; }                       
    virtual void Call(const SReference &, IntelibContinuation &lf) const 
        { lf.RegularReturn(*PTheEmptyList); }            
};

/////////////////////////////////////////////////////////

class SchFunctionQuit : public SExpressionForm {
    IntelibSchemeLoop *the_loop;
public:
    SchFunctionQuit(IntelibSchemeLoop *a_loop) 
        : SExpressionForm(), the_loop(a_loop) {}
    ~SchFunctionQuit() {}
    virtual SString TextRepresentation() const 
        { return "#<FORM QUIT>"; }                       
    virtual void Call(const SReference &params, IntelibContinuation &lf) const 
        {
             the_loop->Break(params);
             lf.RegularReturn(params); 
        }            
};

/////////////////////////////////////////////////////////

class SchFunctionBody : public SExpressionFunction {
public:
    SchFunctionBody() : SExpressionFunction(1,1) {}
    ~SchFunctionBody() {}

    virtual SString TextRepresentation() const 
    { return "#<FUNCTION BODY>"; } 

    virtual void DoApply(int paramsc, const SReference paramsv[], 
                         IntelibContinuation &lf) const
    {
        SchExpressionLambda *f =
            paramsv[0].DynamicCastGetPtr<SchExpressionLambda>();
        if(!f) { 
            lf.RegularReturn(*PTheEmptyList);
            return;
        }
        lf.ReferenceReturn(f->GetBody(), paramsv[0]);
    }            
};


/////////////////////////////////////////////////////////

class SchFunctionLoad : public SExpressionFunction {
    IntelibSchemeLoop *the_loop;
public:
    SchFunctionLoad(IntelibSchemeLoop *a_loop) 
        : SExpressionFunction(1,1), the_loop(a_loop) {}
    ~SchFunctionLoad() {}

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

IntelibSchemeLoop::IntelibSchemeLoop(const SReference& a_package) 
    : package(a_package)
{
    SchSymbol noop("%%%");
    noop->SetGlobalValue(SchReference(new SchFunctionNooperation));
    ImportSymbol(noop);

    SchSymbol quit("QUIT", SchReference(new SchFunctionQuit(this)));
    ImportSymbol(quit);
    ImportSymbol(quit, "%%%QUIT");

    SchSymbol load("LOAD");
    load->SetGlobalValue(SchReference(new SchFunctionLoad(this)));
    ImportSymbol(load);
    ImportSymbol(load, "%%%LOAD");

    SchSymbol body("BODY");
    body->SetGlobalValue(SchReference(new SchFunctionBody));
    ImportSymbol(body);
    ImportSymbol(body, "%%%BODY");

#if 0
#if INTELIB_NILL_TRACE_SUPPORT == 1
    SchSymbol trace("TRACE");
    trace->SetFunction(SchReference(new SchFunctionTrace));
    ImportSymbol(trace);
    ImportSymbol(trace, "%%%TRACE");

    SchSymbol untrace("UNTRACE");
    untrace->SetFunction(SchReference(new SchFunctionUntrace));
    ImportSymbol(untrace);
    ImportSymbol(untrace, "%%%UNTRACE");
#endif
#endif
}

SReference IntelibSchemeLoop::Go(SchemeContinuation *a_lf)
{
    SStreamStdin in;
    SStreamStdout out;
    SStreamStderr err;
    return Go(in, out, err, a_lf);
}

bool IntelibSchemeLoop::ImportSymbol(const SchReference& symb, 
                                   const char *name, 
                                   bool safe)
{
    return 
        static_cast<SchExpressionPackage*>(package.GetPtr())->
            Import(symb, name, safe);
}

SReference IntelibSchemeLoop::Go(const SStreamRef &in, 
                                 const SStreamRef &out, 
                                 const SStreamRef &err, 
                                 SchemeContinuation *lf)
{
    break_flag = false;
    exit_code = SReference();

    if(lf) {
        SReference lex_symbols = lf->GetContext()->GetAllSymbols();
        while(!lex_symbols.IsEmptyList()) {
            SchSymbolRef symb(lex_symbols.Car());
            ImportSymbol(symb);
            lex_symbols = lex_symbols.Cdr();
        }
    }

    SReference the_unspecified;
    if(lf) {
        lf->ReturnUnspecified();
        lf->PopResult(the_unspecified);
    } else {
        SchemeContinuation c;
        c.ReturnUnspecified();
        c.PopResult(the_unspecified);
    }

    SchExpressionPackage *pkg = 
        static_cast<SchExpressionPackage*>(package.GetPtr());

    PTheIntelibReader->SetPackage(pkg);

    do {
        SchReference r;
        try {
            if(PTheIntelibReader->IsReady())
                r = PTheIntelibReader->Get();
            else
                r = intelib_read_line(in, out, err,
                                      *PTheIntelibReader,
                                      package, "nils> ");
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
            SchReference res = 
                lf ? r.Evaluate(*lf) : r.Evaluate();
            if(res.GetPtr() != the_unspecified.GetPtr()) {
                out->Puts(res.TextRepresentation().c_str());
                out->Puts("\n");
            }
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

    return exit_code;

}

SReference IntelibSchemeLoop::Load(const SStreamRef &in, const char *fname)
{
    break_flag = false;
    exit_code = SReference();

    SchExpressionPackage *pkg = 
        static_cast<SchExpressionPackage*>(package.GetPtr());
    PTheIntelibReader->SetPackage(pkg);
    if(fname)
        PTheIntelibReader->SetLine(1, fname);

    do {
        SchReference r = PTheIntelibReader->Read(in);
        if(r==IntelibReader::EofMarker) {
            Break(*PTheSchemeBooleanTrue);
            break;
        }
        try {
            r.Evaluate();
        }
        catch(IntelibX& ex) {
            SchListConstructor L;
            ex.AddStack((L|"#* reading line", PTheIntelibReader->GetLine()));
            throw;
        }
    } while(!break_flag);

    return exit_code;
}

void IntelibSchemeLoop::Break(const SReference &a_exit_code)
{
    break_flag = true;
    exit_code = a_exit_code;
}
