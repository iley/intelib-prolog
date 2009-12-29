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




/*! \file lsymbol.hpp
    \brief Lisp symbol handling

    This module defines LExpressionSymbol and LSymbol classes,
    and defines the appropriate objects for the five Lisp symbols
    provided by the library (NIL, T, QUOTE, FUNCTION and LAMBDA).
 */


#ifndef INTELIB_LSYMBOL_HPP_SENTRY
#define INTELIB_LSYMBOL_HPP_SENTRY


#include "lisp.hpp"
//#include "lcont.hpp"
#include "../sexpress/gensref.hpp"
#include "../genlisp/conteval.hpp"


#ifndef INTELIB_LISP_LIBRARY_DEFINES_SYMBOLS
#define INTELIB_LISP_LIBRARY_DEFINES_SYMBOLS 1
#endif

//! The Lisp symbol S-expression
/*! The class represents the functionality of a Lisp symbol */
class LExpressionSymbol : public SExpressionSpecialToken {
    LReference dynamic_value;
    LReference associated_function;
    bool is_dynamic : 2;
    bool is_constant : 2;
protected:
    LExpressionSymbol(const char *a_name, const IntelibTypeId &t) 
        : SExpressionSpecialToken(t, a_name), 
          dynamic_value(), is_dynamic(false), is_constant(false) {}
    ~LExpressionSymbol() {}

    virtual void Evaluate(class IntelibContinuation& lf);
    virtual void GetApplicable(class SExpressionForm **form,
                               SReference *function,
                               IntelibContinuation&) const;
public:
        //! The type identifier
    static IntelibTypeId TypeId;
        //! The constructor
    LExpressionSymbol(const char *a_name) 
        : SExpressionSpecialToken(TypeId, a_name), 
          dynamic_value(), is_dynamic(false), is_constant(false) {}

        //! Get the dynamic (global) value of the symbol 
    const LReference& GetDynamicValue() const { return dynamic_value; } 
        //! Get the dynamic (global) value by reference
    LReference& GetDynamicValueRef() { return dynamic_value; } 
        //! Set the dynamic (global) value of the symbol 
    void SetDynamicValue(const SReference &val);
        //! Get the associated functional object if any
    const LReference& GetFunction() const { return associated_function; } 
        //! Get the associated functional by reference
    LReference& GetFunctionRef() { return associated_function; } 
        //! Set the associated functional object 
    void SetFunction(const LReference &val) { associated_function = val; }
 
        //! Query if the symbol has dynamic binding style
    bool IsDynamic() const { return is_dynamic; }
        //! Query if the symbol is a constant
    bool IsConstant() const { return is_constant; }
        //! Set binding style
    void SetDynamicBinding(bool dyn = true) { is_dynamic = dyn; }
        //! Set constantness
    void SetConstant(bool cnst = true) { is_constant = cnst; }
};

//! Exception: not a symbol
class IntelibX_lisp_not_a_symbol : public IntelibX {
public:
    IntelibX_lisp_not_a_symbol(SReference a_param);
};

//! Just a reference to a LExpressionSymbol
typedef GenericSReference<LExpressionSymbol,IntelibX_lisp_not_a_symbol>
LSymbolRef;

//! Smart pointer to LExpressionSymbol
/*! This class derived from LReference is dedicated to handle 
    LExpressionSymbols and has the appropriate functionality
 */
class LSymbol : public LSymbolRef {
public:
    LSymbol(const char *s) : LSymbolRef(new LExpressionSymbol(s)) {}
    LSymbol(LExpressionSymbol *ts) : LSymbolRef(ts) {}
    ~LSymbol();

private:
    // made private to prevent the user from mistakes and
    //   to make sure this always points to LExpressionSymbol object.
    void operator =(const LReference& ref) {}
    LSymbol(const LSymbol&) {}
};


//! Symbol naming a library function
/*! This template class is intended to simplify handling 
    Lisp library functions such as CAR, CONS etc. 
    \param F is the class descended from LExpressionForm which represents
     the particular function (the one to be associated with the
     symbol)
 */
template <class F>           // F must be a descender of LExpressionForm
class LFunctionalSymbol : public LSymbol {
public:
    LFunctionalSymbol(const char *name = 0) : LSymbol(name) 
      { (*this)->SetFunction(LReference(::new F())); }
    ~LFunctionalSymbol() {}
private:
    LFunctionalSymbol(const LFunctionalSymbol<F>& t) : LSymbol(0) {}
          /* LSymbol(0) is just to make stupid compiler happy */
};





//! The Lisp symbol QUOTE
class LSymbolQuote : public LSymbol {
public:
    LSymbolQuote();
};

//! The Lisp symbol FUNCTION
class LSymbolFunction : public LSymbol {
public:
    LSymbolFunction();
};

//! The Lisp symbol LAMBDA
class LSymbolLambda : public LSymbol {
public:
    LSymbolLambda();
};

//! The Lisp symbol T (true)
class LSymbolT : public LSymbol {
public:
    LSymbolT();
};

//! The Lisp symbol NIL (empty list)
class LSymbolNIL : public LSymbol {
public:
    LSymbolNIL();
};

//! Encapsulating object for the symbols provided by the library
extern struct LLibraryProvidedSymbols {
    LSymbolQuote Quote;
    LSymbolFunction Function;
    LSymbolLambda Lambda;
    LSymbolT SymT;
    LSymbolNIL SymNil;
} TheLibraryProvidedSymbols;

#if INTELIB_LISP_LIBRARY_DEFINES_SYMBOLS == 1
#define QUOTE (TheLibraryProvidedSymbols.Quote)
#define FUNCTION (TheLibraryProvidedSymbols.Function)
#define LAMBDA (TheLibraryProvidedSymbols.Lambda)
#define T (TheLibraryProvidedSymbols.SymT)
#define NIL (TheLibraryProvidedSymbols.SymNil)
#endif

//! Exception: attempt to get a value of unassinged symbol
class IntelibX_symbol_has_no_value : public IntelibX {
public:
    IntelibX_symbol_has_no_value(SReference a_param);
};

//! Exception: attempt to assign a value to a constant symbol
class IntelibX_cant_change_constant : public IntelibX {
public:
    IntelibX_cant_change_constant(SReference a_param);
};


#endif // sentry
