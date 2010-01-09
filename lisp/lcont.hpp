//   InteLib                                    http://www.intelib.org
//   The file lisp/lcont.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*! \file lcont.hpp
    \brief The lisp-specific continuation

    This module defines a Lisp-specific version of the InteLib
    continuation-based virtual machine (the LispContinuation class),
    along with the Lisp-specific lexical context (the LExpressionContext
    class).
 */

#ifndef INTELIB_LCONT_HPP_SENTRY
#define INTELIB_LCONT_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/iexcept.hpp"
#include "../sexpress/gensref.hpp"
#include "../genlisp/conteval.hpp"
#include "../genlisp/binds.hpp"
#include "lisp.hpp"


class LExpressionSymbol;

class LExpressionContext;

    //! Not a lisp context
class IntelibX_lisp_not_a_context : public IntelibX {
public:
    IntelibX_lisp_not_a_context(SReference a_param);
};

//! A reference to a context
typedef GenericSReference<LExpressionContext, IntelibX_lisp_not_a_context> 
    LContextRef;

//! Lexical context
/*! The class encapsulates lisp-specific lexical context. */
class LExpressionContext : public SExpression {  
    LContextRef prev;
    IntelibBindingsSet valtbl, funtbl;
public:
        //! The type identifier
    static IntelibTypeId TypeId;
        //! Makes an empty context
    LExpressionContext();
        //! Makes a context atop of an existing context
    LExpressionContext(LContextRef a_prev); 
protected:
    ~LExpressionContext();    

public:
        //! Bind the symbol to the value
    void AddBinding(const LExpressionSymbol *symb, const SReference& val);
        //! Lookup for a binding
        /*! Find the appropriate binding and return a pointer
            to its value-storing slot.  In case no binding is
            found, return NULL
         */
    SReference* GetBinding(const LExpressionSymbol *symb) const;
        //! Find or create a binding
        /*! Find the appropriate binding and return a pointer
            to its value-storing slot.  In case no binding is
            found, create it and return the pointer.
         */
    SReference* ProvideBinding(const LExpressionSymbol *symb);

        //! Associate a function with a symbol in the lexical manner
    void AddFunBinding(const LExpressionSymbol *symb, const SReference& val);
        //! Lookup for a functional binding
        /*! Find the appropriate functional binding and return a pointer
            to its value-storing slot.  In case no binding is
            found, return NULL
         */
    SReference* GetFunBinding(const LExpressionSymbol *symb) const;
        //! Find or create a functional binding
        /*! Find the appropriate functionall binding and return a pointer
            to its value-storing slot.  In case no binding is
            found, create it and return the pointer.
         */
    SReference* ProvideFunBinding(const LExpressionSymbol *symb);

        //! Make a list of all bound symbols
    SReference GetAllSymbols() const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual class SString TextRepresentation() const;
#endif
};

//! Lisp-specific continuation-based virtual machine
/*! This class determines the Lisp computation model
    providing the method JustEvaluate() and overriding
    the method CustomCommand() so that it handles to
    instruction codes missing in the base class.  The
    specific context handling is also provided.
 */
class LispContinuation : public IntelibContinuation {
public:
        //! The default constructor
    LispContinuation() : IntelibContinuation() {
        PTheTrueValue = PTheLispBooleanTrue;
        PTheFalseValue = PTheLispBooleanFalse;
    }

        //! The CALL/CC-related constructor
    LispContinuation(const LispContinuation &other, bool b)
        : IntelibContinuation(other, b)
    {
        PTheTrueValue = PTheLispBooleanTrue;
        PTheFalseValue = PTheLispBooleanFalse;
    }


        //! The destructor
    ~LispContinuation() {}

        //! additional instructions
    enum CustomInstructions {
            //! pop a result and evaluate it as a form
        take_result_as_form = max_command - 1,
            //! pop a result and then push it twice
        duplicate_last_result = max_command - 2
     };

        //! Get the current lexical context
    LContextRef GetContext() const
        { return IntelibContinuation::GetContext(); }

        //! Set the current lexical context
    void SetContext(const LContextRef& context)
        { IntelibContinuation::SetContext(context); }

private:
        //! This version determines the Lisp evaluation model
    virtual void EvaluateForm(const SExpressionCons* form);
        //! Handle the two custom command
    virtual void CustomCommand(int opcode, const SReference& param);

#if 0
    SReference& GetSymbolValue(LExpressionSymbol *sym) const;
#endif
};

//! Not a lisp function
class IntelibX_lisp_not_a_function : public IntelibX {
public:
    IntelibX_lisp_not_a_function(SReference a_param);
};

//! Lisp symbol has no value
class IntelibX_lisp_symbol_has_no_value : public IntelibX {
public:
    IntelibX_lisp_symbol_has_no_value(SReference a_param);
};

//! Exception: no function is associated with the expression
/*! The first element of a form being evaluated doesn't specify a 
    Lisp function, that is, it is not a lambda-list, and it is not 
    a symbol which has an associated function (either dynamic or lexical)
 */
class IntelibX_no_associated_function : public IntelibX {
public:
    IntelibX_no_associated_function(SReference a_param);
};

#endif
