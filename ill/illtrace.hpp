//   InteLib                                    http://www.intelib.org
//   The file ill/illtrace.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




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
