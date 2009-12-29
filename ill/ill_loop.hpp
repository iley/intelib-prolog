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




#ifndef INTELIB_ILL_LOOP_HPP_SENTRY
#define INTELIB_ILL_LOOP_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../tools/sstream.hpp"
#include "../lisp/lisp.hpp"

class IntelibContinuation;

class IntelibLispLoop {
    bool break_flag;
    SReference exit_code;
    SReference package;
public:
    IntelibLispLoop(const SReference& a_package);
    ~IntelibLispLoop() {}
    
    bool ImportSymbol(const LReference& symb, 
                      const char *name = 0, 
                      bool safe = false);

    SReference Go(IntelibContinuation *a_lf = 0);
    SReference Go(const SStreamRef &in, 
                  const SStreamRef &out, 
                  const SStreamRef &err,
                  IntelibContinuation *a_lf = 0);

    SReference Load(const SStreamRef &in, const char *fname = 0);

    void Break(const SReference &a_exit_code);
    void Unbreak() { break_flag = false; }
};

class SString;

SString stack_representation(SReference stack);

#endif // sentry
