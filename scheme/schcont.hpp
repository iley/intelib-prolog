//   InteLib                                    http://www.intelib.org
//   The file scheme/schcont.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*! \file schcont.hpp
    \brief Scheme Cntinuation

    This module defines the Scheme-specific version of the
    continuation-based virtual machine.  See the SchemeContinuation
    class description for details.
 */
#ifndef INTELIB_SCHCONT_HPP_SENTRY
#define INTELIB_SCHCONT_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/iexcept.hpp"
#include "../sexpress/gensref.hpp"
#include "../genlisp/conteval.hpp"
#include "../genlisp/binds.hpp"
#include "scheme.hpp"


class SchExpressionSymbol;

class SchExpressionContext;

//! Not a Scheme context object
class IntelibX_scheme_not_a_context : public IntelibX {
public:
    IntelibX_scheme_not_a_context(SReference a_param);
};

//! A simple reference to SchExpressionContext
typedef GenericSReference<SchExpressionContext, IntelibX_scheme_not_a_context> 
    SchContextRef;

//! Lexical context
/*! The class encapsulates Scheme lexical context.
    \note In the InteLib implementation of Scheme, the 'outermost'
    context is represented by values stored within the symbols'
    objects, just like 'dynamic' values in Lisp.  This accelerates
    access to library functions and therefore increases the overall
    execution speed.  So, the SchExpressionContext objects only
    represent local contexts such as contexts created by functions
    or by LET.
 */
class SchExpressionContext : public SExpression {  
    SchContextRef prev;
    IntelibBindingsSet tbl;
public:
        //! The type identifier
    static IntelibTypeId TypeId;

        //! The default constructor creates an empty context
    SchExpressionContext(); 
        //! Creates one context ('inner one') atop of another ('outer')
    SchExpressionContext(SchContextRef a_prev); 
protected:
    ~SchExpressionContext();    

public:
        //! Bind the symbol to the value
    void AddBinding(const SchExpressionSymbol *symb, const SReference& val);

        //! Lookup for a binding
        /*! Find the appropriate binding and return a pointer
            to its value-storing slot.  In case no binding is
            found, return NULL
         */
    SReference* GetBinding(const SchExpressionSymbol *symb) const;
        //! Find or create a binding
        /*! Find the appropriate binding and return a pointer
            to its value-storing slot.  In case no binding is
            found, create it and return the pointer.
         */
    SReference* ProvideBinding(const SchExpressionSymbol *symb);

        //! Make a list of all bound symbols
    SReference GetAllSymbols() const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual class SString TextRepresentation() const;
#endif
};


//! Scheme-specific continuation-based virtual machine
/*! This class determines the Scheme computation model
    providing the method EvaluateForm() and overriding
    the method CustomCommand() so that it handles to
    instruction codes missing in the base class.  The
    specific context handling is also provided.
 */
class SchemeContinuation : public IntelibContinuation {
public:
        //! The default constructor
    SchemeContinuation() : IntelibContinuation() {
        PTheTrueValue = PTheSchemeBooleanTrue;
        PTheFalseValue = PTheSchemeBooleanFalse;
    }

        //! The CALL/CC-related constructor
    SchemeContinuation(const SchemeContinuation &other, bool b)
        : IntelibContinuation(other, b)
    {
        PTheTrueValue = PTheSchemeBooleanTrue;
        PTheFalseValue = PTheSchemeBooleanFalse;
    }

        //! The destructor
    ~SchemeContinuation() {}

        //! additional instructions
    enum CustomInstructions { case_check = max_command - 1 };

        //! Get the current context
    SchContextRef GetContext() const
        { return IntelibContinuation::GetContext(); }

        //! Set the current context
    void SetContext(const SchContextRef& context)
        { IntelibContinuation::SetContext(context); }

        //! Get the value using the current context if any
    SReference& GetSymbolValue(SchExpressionSymbol *sym) const;
    SReference GetSymbolValue(const SchExpressionSymbol *sym) const;

private:
    virtual void EvaluateForm(const SExpressionCons *form);
    virtual void CustomCommand(int opcode, const SReference& param);

    void CaseCheck(const SReference& expr);
};

//! Scheme symbol has no value
class IntelibX_scheme_symbol_has_no_value : public IntelibX {
public:
    IntelibX_scheme_symbol_has_no_value(SReference a_param);
};

#endif
