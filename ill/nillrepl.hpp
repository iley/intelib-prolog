#ifndef INTELIB_NILLREPL_HPP_SENTRY
#define INTELIB_NILLREPL_HPP_SENTRY

#include "../interact/repl.hpp"

class IntelibLispLoop : public IntelibRepl {
public:
    IntelibLispLoop(const SReference& a_package)
        : IntelibRepl(a_package) {}
    virtual ~IntelibLispLoop() {}
    
    virtual bool ImportSymbol(const SReference& symb, 
                              const char *name = 0, 
                              bool safe = false);

    virtual void ImportLexicalSymbols(IntelibContinuation *lf);
};

#endif
