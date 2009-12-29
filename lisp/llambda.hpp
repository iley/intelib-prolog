// +-------------------------------------------------------------------------+
// |               I__n__t__e__L__i__b           0.6.21 development          |
// | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
// |                                                                         |
// | This is free software. The library part is available under              |
// |                               GNU LESSER GENERAL PUBLIC LICENSE v.2.1.  |
// | GNU LGPL v2.1 is found in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
// |     Please see also docs/readme.txt and visit http://www.intelib.org    |
// |                                                                         |
// | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
// +-------------------------------------------------------------------------+




/*! \file llambda.hpp
    \brief Lisp function objects

    This module defines several classes to represent functional (that is,
    applicable) objects in Lisp, such as closures.  Macro (in the sence of
    Common Lisp) is also an applicable object in InteLib Lisp, and it is
    represented by the LExpressionMacro class here.
 */

#ifndef INTELIB_LLAMBDA_HPP_SENTRY
#define INTELIB_LLAMBDA_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/iexcept.hpp"
#include "../genlisp/lispform.hpp"
#include "lcont.hpp"

class LExpressionSymbol;

//! Lambda-list and a list of forms
/*! This class represents the lambda-list and a body.
    It is used both for implementation of functions and macros.
 */
class LispLambdaBody {
protected:
    LContextRef context;
    SReference lambda_list;
    LExpressionSymbol **lambda_vector;
    LExpressionSymbol *lambda_rest;
    int lambda_length;
    SReference body;

    LispLambdaBody(const LContextRef &a_cont,
                   const SReference &lambda, const SReference& body);
    ~LispLambdaBody();

    void DoAnalyseLambdaList(const SReference &rest, int n);

public:
        //! Get the body
    SReference& GetBody() { return body; }
};

//! A Lisp function written in Lisp
/*! This object represents a closure, that is, the thing created
    by #'(lambda (...) ...), or by DEFUN, or FLET.
 */
class LExpressionLambda : public SExpressionFunction, public LispLambdaBody {
public:
        //! The type identifier
    static IntelibTypeId TypeId;
        //! The constructor
        /*! \param a_cont is the lexical context within which
                   the closure is created
            \param lambda is the list of symbols to be used as
                   formal parameters
            \param body is the list of forms that is to be used
                   as the function's body
         */
    LExpressionLambda(const LContextRef &a_cont,
                      const SReference &lambda, const SReference& body);
protected:
    virtual ~LExpressionLambda();

public:
        //! Perform application to the given parameters
        /*! This method actually does all the job; however,
            you'd better use the SExpressionFunction::Apply()
            method as it is more general.  Furthermore, you
            generally won't need to do even so; better construct
            the appropriate form and LReference::Evaluate() it.
            This is because the application is actually only
            scheduled to be done within the given continuation,
            and you'll need yourself to make the continuation run
            to get the result of the application.
         */
    virtual void DoApply(int paramc,
                         const SReference *paramv,
                         IntelibContinuation &cont) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif
};

//! A macro (in the sence of Common Lisp)
/*! InteLib models the Common Lisp's flavor of macros.  They aren't
    really macros though because they're always expanded at the
    execution time, so they perhaps should be terribly slow.
    Actually, the LExpressionMacro is a kind of special form
    which, like a Lambda, has a lambda list, a body and acts as
    a closure (remembering all the lexical bindings that were in
    effect on the moment the macro has been created).  A call to
    such a macro evaluates as follows:
        - a context is made atop the context remembered by the macro;
          the formal parameters are bound to the respective values
          from the call, as such (without evaluation);
        - the body of the macro is evaluated within the resulting
          context, and the result is stored
        - the caller's context is restored
        - the previously stored result is evaluated as a form
          within the caller context
 */
class LExpressionMacro : public SExpressionForm, public LispLambdaBody {
public:
        //! The type identifier
    static IntelibTypeId TypeId;
        //! The constructor
        /*! \param a_cont is the lexical context within which
                   the closure is created
            \param lambda is the list of symbols to be used as
                   formal parameters
            \param body is the list of forms that is to be used
                   as the function's body
         */
    LExpressionMacro(const LContextRef &a_cont,
                     const SReference &lambda, const SReference& body);

protected:
    virtual ~LExpressionMacro();

public:
        //! Perform macro expansion and evaluate the result
        /*! This method actually does all the job; however, you
            generally won't need to do so; better construct
            the appropriate form and LReference::Evaluate() it.
            This is because the expansion and evaluation is actually
            only scheduled to be done within the given continuation,
            and you'll need yourself to make the continuation run
            to get the result of the application.
         */
    virtual void Call(const SReference &formtail,
                      IntelibContinuation &cont) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif
};



#endif
