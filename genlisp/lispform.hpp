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




/*! \file lispform.hpp
    \brief The basic classes for applicable forms both for Lisp and Scheme

    See the classes SExpressionFunction and SExpressionForm for details.
 */

#ifndef INTELIB_LISPFORM_HPP_SENTRY
#define INTELIB_LISPFORM_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/iexcept.hpp"

class IntelibContinuation;

//! Generic function
/*! An S-expression which can be applied to a number of arguments
    that are to be evaluated before the function is actually called.
    A function is created defining a child class of SExpressionFunction
    in which the DoApply method is to be specified as the method
    which performs the evaluation.  If text representations are
    enabled, then the TextRepresentation method is to be specified
    as well as it is left pure virtual by this class.
 */
class SExpressionFunction : public SExpression {
protected:
    int min_param, max_param;
public:
    static IntelibTypeId TypeId;
    SExpressionFunction(int min, int max)
        : SExpression(TypeId), min_param(min), max_param(max) {}
protected:
    SExpressionFunction(const IntelibTypeId& id, int min, int max)
        : SExpression(id), min_param(min), max_param(max) {}
    virtual ~SExpressionFunction() {}
public:
    void Apply(int paramc, const SReference *paramv,
               IntelibContinuation &cont) const;
protected:
    virtual void DoApply(int paramc,
                         const SReference *paramv,
                         IntelibContinuation &cont) const = 0;
};


//! A C++ function that can be used in Lisp/Scheme expressions
typedef SReference (*UserLispFunction)(int, const SReference *);


//! UserLispFunction instances are wrapped by this class
/*! Such a function is a closure, too, but is closed by
    the empty context
 */
class SExpressionUserCFunction : public SExpressionFunction {
    UserLispFunction fun;
public:
        //! The type identifier
    static IntelibTypeId TypeId;
        //! The constructor
    SExpressionUserCFunction(UserLispFunction a)
        : SExpressionFunction(TypeId, -1, -1), fun(a) {}
        //! The method to do the job
    virtual void DoApply(int paramsc, 
                         const SReference *paramsv,
                         IntelibContinuation &lf) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif
protected:
    ~SExpressionUserCFunction() {}
};



//! Generic special form
/*! An object which represents a special form (or a special syntax).
    Most implementations do not represent such forms with any
    S-expressions; the InteLib implementation does.
    A form is created defining a child class of SExpressionForm in
    which the Call method must be defined. If text representations are
    enabled, then the TextRepresentation method is to be specified
    as well as it is left pure virtual by this class.
 */
class SExpressionForm : public SExpression {
public:
    static IntelibTypeId TypeId;
    SExpressionForm() : SExpression(TypeId) {}
protected:
    SExpressionForm(const IntelibTypeId& id) : SExpression(id) {}
    virtual ~SExpressionForm() {}

public:
    virtual void Call(const SReference &formtail,
                      IntelibContinuation &cont) const = 0;
};

//! Exception: too many parameters in a call
class IntelibX_too_many_params : public IntelibX {
public:
    IntelibX_too_many_params(SReference a_param);
};

//! Exception: too few parameters in a call
class IntelibX_too_few_params : public IntelibX {
public:
    IntelibX_too_few_params(SReference a_param);
};

//! Exception: invalid argument set
class IntelibX_invalid_arguments : public IntelibX {
public:
    IntelibX_invalid_arguments(SReference a_param);
};

#endif
