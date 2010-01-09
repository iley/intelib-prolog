//   InteLib                                    http://www.intelib.org
//   The file scheme/schsymb.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*! \file schsymb.hpp
    \brief Scheme symbols

    Defines the SchExpressionSymbol class, a smart pointer to it and the
    framework for labels provided by the library.  These are the symbol
    QUOTE and the labels for (), #T and #F.
 */
#ifndef INTELIB_SCHSYMB_HPP_SENTRY
#define INTELIB_SCHSYMB_HPP_SENTRY

#include "../sexpress/gensref.hpp"
#include "../sexpress/sexpress.hpp"
#include "../genlisp/conteval.hpp"
#include "../genlisp/lispform.hpp"    // for UserLispFunction
#include "scheme.hpp"

//! Scheme symbol
/*! In Scheme, the symbol is only associated with one value, be it
    a data expression or a function.  Although in Scheme there's no
    global/dynamic values (such as in Lisp), in InteLib Scheme the
    "root lexical environment" is implemented storing the values
    within the symbol objects, just like in Lisp.
 */
class SchExpressionSymbol : public SExpressionSpecialToken {
    SReference value;
protected:
        //! The constructor for derived types
    SchExpressionSymbol(const char *a_name, const IntelibTypeId &t) 
        : SExpressionSpecialToken(t, a_name), value() {}
    ~SchExpressionSymbol() {}

    virtual void Evaluate(class IntelibContinuation& lf);
    virtual void GetApplicable(class SExpressionForm **form,
                               SReference *function,
                               IntelibContinuation&) const;
public:
        //! The type identifier
    static IntelibTypeId TypeId;
        //! The simple constructor
    SchExpressionSymbol(const char *a_name) 
        : SExpressionSpecialToken(TypeId, a_name), value() {}

        //! Get the global value of the symbol 
    const SReference& GetGlobalValue() const { return value; } 
        //! Get the location of the global value  
    SReference& GetGlobalValue() { return value; } 
        //! Set the global value of the symbol 
    void SetGlobalValue(const SReference &val) { value = val; }
    void SetGlobalValue(UserLispFunction fun) { value = SchReference(fun); }
};

//! Exception: not a symbol
class IntelibX_scheme_not_a_symbol : public IntelibX {
public:
    IntelibX_scheme_not_a_symbol(SReference a_param);
};

//! Just a reference to a SchExpressionSymbol
typedef GenericSReference<SchExpressionSymbol,IntelibX_scheme_not_a_symbol>
SchSymbolRef;

//! Steering pointer to SchExpressionSymbol
/*! This class derived from SReference is dedicated to handle 
    SchExpressionSymbols and has the appropriate functionality;
    unlike the SchSymbolRef, this class creates an object of
    the symbol and keeps it on a lifetime basis.
 */
class SchSymbol : public SchSymbolRef {
public:
        //! Default constructor just makes the symbol
    SchSymbol(const char *s) 
        : SchSymbolRef(new SchExpressionSymbol(s)) {}

        //! Create a symbol and bind a global value to it
    SchSymbol(const char *s, const SReference& val)
        : SchSymbolRef(new SchExpressionSymbol(s))
    {
        (*this)->SetGlobalValue(val);
    }

        //! The destructor
    ~SchSymbol() {}

private:
    // made private to prevent the user from mistakes and
    //   to make sure this always points to SchExpressionSymbol object.
    void operator =(const SchSymbol& ref) {}
    SchSymbol(const SchSymbol&) {}
};

//! The symbol QUOTE for InteLib Scheme
class SchSymbolQuote : public SchSymbol {
public:
    SchSymbolQuote();
};

//! The Scheme object of #T
class SchLabelTrue : public SLabel {
public:
    SchLabelTrue();
};

//! The Scheme object of #F
class SchLabelFalse : public SLabel {
public:
    SchLabelFalse();
};

//! The Scheme object for empty list
class SchLabelEmptyList : public SLabel {
public:
    SchLabelEmptyList();
};

//! The global Scheme symbols and labels provided by the library
extern struct SchLibraryProvidedSymbols {
    SchSymbolQuote Quote;
    SchLabelTrue True;
    SchLabelFalse False;
    SchLabelEmptyList EmptyList;
} TheSchLibraryProvidedSymbols;

extern SchSymbol &QUOTE;

#endif
