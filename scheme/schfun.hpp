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




/*! \file schfun.hpp
    \brief Scheme function objects

    This module defines several classes to represent functional (that is,
    applicable) objects in Scheme, such as closures.  See the
    SchExpressionLambda class description for details.
 */

#ifndef INTELIB_SCHFUN_HPP_SENTRY
#define INTELIB_SCHFUN_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/iexcept.hpp"
#include "../genlisp/lispform.hpp"
#include "schcont.hpp"

class SchExpressionSymbol;

//! A Scheme closure (that is, a function written in Scheme)
class SchExpressionLambda : public SExpressionFunction {
    SchContextRef context;
    SReference lambda_list;
    SchExpressionSymbol **lambda_vector;
    SchExpressionSymbol *lambda_rest;
    SReference body;
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
    SchExpressionLambda(const SchContextRef &a_cont,
                        const SReference &lambda, const SReference& body);
protected:
    virtual ~SchExpressionLambda();

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

        //! Get the body
    SReference& GetBody() { return body; }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

private:
    void DoAnalyseLambdaList(const SReference &rest, int n);
};

#endif
