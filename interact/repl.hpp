//   InteLib                                    http://www.intelib.org
//   The file interact/repl.hpp, formerly known as ill/ill_loop.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2010
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#ifndef INTELIB_REPL_HPP_SENTRY
#define INTELIB_REPL_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../tools/sstream.hpp"

class IntelibContinuation;

class IntelibRepl {
    bool break_flag;
    bool *extra_break_flag;
    SReference exit_code;
protected:
    SReference package;
public:
    IntelibRepl(const SReference& a_package);
    virtual ~IntelibRepl() {}
    
protected:
    virtual bool ImportSymbol(const SReference& symb, 
                              const char *name = 0, 
                              bool safe = false) = 0;
    virtual void ImportLexicalSymbols(IntelibContinuation *lf) = 0;
    virtual void MakeFunctionalSymbol(SReference function,
                                      const char *name,
                                      const char *second_name = 0) = 0;
    virtual class IntelibReader *MakeLocalReader() const = 0;
    virtual class IntelibContinuation *MakeLocalContinuation() const = 0;
    virtual SString SpecificTextRepresentation(const SReference &r) const = 0;

    void AddCommonSymbols();

public:
    SReference Go(IntelibContinuation *a_lf = 0);
    SReference Go(const SStreamRef &in, 
                  const SStreamRef &out, 
                  const SStreamRef &err,
                  IntelibContinuation *a_lf = 0);

    SReference Load(const SStreamRef &in, const char *fname = 0);

    void Break(const SReference &a_exit_code);
    void Unbreak() { break_flag = false; }
    void SetExtraBreakFlag(bool *a_flag = 0) { extra_break_flag = a_flag; }
};

class SString;

SString stack_representation(SReference stack);

#endif // sentry
