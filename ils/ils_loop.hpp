//   InteLib                                    http://www.intelib.org
//   The file ils/ils_loop.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#ifndef INTELIB_ILS_LOOP_HPP_SENTRY
#define INTELIB_ILS_LOOP_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../tools/sstream.hpp"
#include "../scheme/scheme.hpp"

class SchemeContinuation;

class IntelibSchemeLoop {
    bool break_flag;
    SReference exit_code;
    SReference package;
public:
    IntelibSchemeLoop(const SReference& a_package);
    ~IntelibSchemeLoop() {}
    
    bool ImportSymbol(const SchReference& symb, 
                      const char *name = 0, 
                      bool safe = false);

    SReference Go(SchemeContinuation *a_lf = 0);
    SReference Go(const SStreamRef &in, 
                  const SStreamRef &out, 
                  const SStreamRef &err,
                  SchemeContinuation *a_lf = 0);

    SReference Load(const SStreamRef &in, const char *fname = 0);

    void Break(const SReference &a_exit_code);
    void Unbreak() { break_flag = false; }
};

#endif // sentry
