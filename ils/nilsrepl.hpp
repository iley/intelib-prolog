//   InteLib                                    http://www.intelib.org
//   The file ils/nilsrepl.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2010
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.


#ifndef INTELIB_NILSREPL_HPP_SENTRY
#define INTELIB_NILSREPL_HPP_SENTRY

#include "../interact/repl.hpp"

class IntelibSchemeLoop : public IntelibRepl {
public:
    IntelibSchemeLoop(const SReference& a_package);
    virtual ~IntelibSchemeLoop() {}
    
    virtual bool ImportSymbol(const SReference& symb, 
                              const char *name = 0, 
                              bool safe = false);

    virtual void ImportLexicalSymbols(IntelibContinuation *lf);

private:
    virtual class IntelibReader *MakeLocalReader() const;
    virtual class IntelibContinuation *MakeLocalContinuation() const;
    virtual SString SpecificTextRepresentation(const SReference &r) const;
    virtual void MakeFunctionalSymbol(SReference function,
                                      const char *name,
                                      const char *second_name = 0);
};

#endif
