//   InteLib                                    http://www.intelib.org
//   The file lisp/lisp.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*! \file lisp.hpp
    \brief The primary classes for the Lisp evaluation model

    This module defines the classes LReference, LListConstructor,
    LFunctionConstructor, along with global variables and some other
    stuff related to Lisp evaluation model.
 */

#ifndef INTELIB_LISP_HPP_SENTRY
#define INTELIB_LISP_HPP_SENTRY


#include "../sexpress/sexpress.hpp"
#include "../sexpress/iexcept.hpp"
#include "../genlisp/lispform.hpp"    // for UserLispFunction


class LReference;

//! Reference to a Lisp S-expression
/*! This class only differs from SReference in that is has 
    evaluation-related functions, the operator~(), the IsTrue(),
    IsEql() and IsEqual() functions. 
      \par
    If you don't need to evaluate an S-expression, it is better to use
    SReference as it is accepted by more function than LReference.  They
    can be cast to each other though.
 */
class LReference : public SReference {
public:
        //! Default constructor creates an unbound reference
    LReference() {}
        //! Copy constructor
    LReference(const LReference& ref) : SReference(ref) {}
        //! Cast constructor
    LReference(const SReference& ref) : SReference(ref) {}
        //! Cast from a plain pointer
    LReference(SExpression* p) : SReference(p) {}
        //! Cast from UserLispFunction
    LReference(UserLispFunction fun);
        //! Make a cons
    LReference(const SReference& car, const SReference& cdr)
        : SReference(car, cdr) {}
 
    template <class X> LReference(X &x) : SReference(x) {}
    template <class X> LReference(const X &x) : SReference(x) {}

    ~LReference() {}

        //! Evaluate the expression
        /*! This method creates a local object of LispContinuation
            and uses it to evaluate the expression.  Consider it
            as a top-level form evaluation
         */
    LReference Evaluate() const;

        //! Evaluate the expression within an existing machine
        /*! The given LispContinuation's state is marked;
            then, the object is used to evaluate the expression
            (that is, *this is scheduled to be evaluated
            within that LispContinuation object, and the evaluation
            is launched to run until the marked state is reached again).
            \warning In most cases this leaves the continuation
            exactly in the same state as it was in before the call;
            this is not guaranteed though.
         */
    LReference Evaluate(class LispContinuation &within) const; 

        //! Apply the expression to the list of arguments
        /*! This method creates a local object of LispContinuation
            and uses it to apply the expression, which must be a
            function, to the given arguments.
         */
    LReference Apply(const SReference &args) const;

        //! Apply within the given continuation
        /*! This method applies the object, which must be a function,
            to the given arguments within the given continuation,
            just like the respective Evaluate() method does.
         */
    LReference Apply(const SReference &args,
                     class LispContinuation &within) const; 

        //! Is the object 'true' in the sence of Lisp
        /*! Returns false in case the object stores the same
            pointer as PTheLispBooleanFalse variable does;
            otherwise, returns true.
         */
    bool IsTrue() const; 

        //! Add another element to list
    LReference& operator,(const SReference& ref)
        { SReference::operator,(ref); return *this; }

        //! Set the cdr of the last pair of list
    LReference& operator||(const SReference& ref)
        { SReference::operator||(ref); return *this; }

        //! Make the list (QUOTE *this)
    LReference operator~() const;

        //! Is the object EQL (in the sence of CL) to the other
    bool IsEql(const SReference& other) const;
        //! Is the object EQUAL (in the sence of CL) to the other
    bool IsEqual(const SReference& other) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
        //! Text representation
        /*! This version differs from SExpression::TextRepresentation()
            in that it handles the QUOTE and FUNCTION forms
         */
    SString TextRepresentation() const;
#endif
};


    //! Pointer to the LAMBDA symbol 
extern SReference *PTheLispSymbolLambda;
    //! Pointer to the QUOTE symbol
extern SReference *PTheLispSymbolQuote;
    //! Pointer to the FUNCTION symbol
extern SReference *PTheLispSymbolFunction;
    //! Pointer to a boolean true constant
extern SReference *PTheLispBooleanTrue;
    //! Pointer to the boolean false object
extern SReference *PTheLispBooleanFalse;


inline bool LReference::IsTrue() const 
{ return GetPtr() != PTheLispBooleanFalse->GetPtr(); } 

inline LReference LReference::operator~() const 
{ 
    return 
        LReference(
            new SExpressionCons(*PTheLispSymbolQuote, 
                                SReference(
                                    new SExpressionCons(*this, 
                                                        *PTheEmptyList)
                               ))
       ); 
} 



//! Lisp List constructor
/*! This class is similar to SListConstructor, and is used to create Lisp
    lists. The difference is that
      - this class' operators return LReference instead of SReference 
      - this class has the operator~() which represents the Lisp QUOTE
        abbreviation (actually, this is used to represent empty lists, 
        like ` ~(L) ').
    \note Altough it looks senseless to derive this class from
        SListConstructor (as it doesn't use anything from it's parent),
        the derivation makes some sence because SListConstructor object is 
        widely used as a representation of empty list (e.g., there's the
        appropriate SReference class constructor)
*/
class LListConstructor : public SListConstructor {
public:
    LListConstructor() {}
    ~LListConstructor() {}
    
    LReference operator |(const LReference &t) const
        { return LReference(new SExpressionCons(t, *PTheEmptyList)); } 
    LReference operator ^(const LReference &other) const
        { return LReference(new SExpressionCons(*PTheEmptyList, other)); }

    LReference operator ~() const { return *PTheEmptyList; }
};


//! Lisp Function abbreviation representer
/*! Just like SListConstructor and LListConstructor, the
    LFunctionConstructor class is not a datastructure: it is intended to
    create one more unary operation. The operation represents the
    well-known Lisp abbreviation for (FUNCTION A), which is #'A.
    The Lisp token #'X is translated to C++ as F^X, where F is an 
    instance of the LFunctionConstructor class.
 */
class LFunctionConstructor {
public:
    LFunctionConstructor() {}
    ~LFunctionConstructor() {}
    
    LReference operator ^(const LReference &op) const
        { return SReference(*PTheLispSymbolFunction, 
                            SReference(op, *PTheEmptyList)); }
};

//! Lisp quotation
/*! As usual, operator~() is used in InteLib to represent Lisp apostrophe.
    This particular version is created for the case we try to quote an
    expression represented with SReference object, which has no operator~()
 */
inline LReference operator~(const SReference& r) { return ~LReference(r); }


#endif
