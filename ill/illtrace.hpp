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




// ///////////////////////////////////////////////////////////////////
// Trace support for NILL
// The initial version was contributed by Olga Shtefan a.k.a. Intafy
//     (http://www.intelib.org/cgi-bin/wiki/awki.cgi/IntAfy)
// ///////////////////////////////////////////////////////////////////

#ifndef INTELIB_ILLTRACE_HPP_SENTRY
#define INTELIB_ILLTRACE_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../genlisp/lispform.hpp"
#include "../lisp/lisp.hpp"

class LFunctionTrace : public SExpressionForm {
public:
    virtual void Call(const SReference &, IntelibContinuation &lf) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif
};

class LFunctionUntrace : public SExpressionForm {
public:
    virtual void Call(const SReference &, IntelibContinuation &lf) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

};

#endif
