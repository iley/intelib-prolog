//   InteLib                                    http://www.intelib.org
//   The file scheme/scheme.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*! \file scheme.hpp
    \brief The primary classes for the Scheme evaluation model

    This module defines the classes SchReference and SchListConstructor
    along with global variables and some other stuff related to Scheme
    evaluation model.
 */
#ifndef INTELIB_SCHEME_HPP_SENTRY
#define INTELIB_SCHEME_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../genlisp/lispform.hpp"


//! Reference to a Scheme S-expression
/*! This class only differs from SReference in that is has 
    evaluation-related functions, the operator~(), the IsTrue(),
    IsEql() and IsEqual() functions. 
      \par
    If you don't need to evaluate an S-expression, it is better to use
    SReference as it is accepted by more function than SchReference.  They
    can be cast to each other though.
 */
class SchReference : public SReference {
public:
        //! Default constructor creates an unbound reference
    SchReference() {}
        //! Copy constructor
    SchReference(const SchReference& ref) : SReference(ref) {}
        //! Cast constructor
    SchReference(const SReference& ref) : SReference(ref) {}
        //! Cast from a plain pointer
    SchReference(SExpression* p) : SReference(p) {}

    SchReference(UserLispFunction fun);

        //! Make a cons
    SchReference(const SReference& car, const SReference& cdr)
        : SReference(car, cdr) {}
 
    template <class X> SchReference(X &x) : SReference(x) {}
    template <class X> SchReference(const X &x) : SReference(x) {}

        //! The destructor
    ~SchReference() {}

        //! Evaluate the expression
        /*! This method creates a local object of SchemeContinuation
            and uses it to evaluate the expression.  Consider it
            as a top-level form evaluation.
         */
    SchReference Evaluate() const; 
        //! Evaluate the expression within an existing machine
        /*! The given SchemeContinuation's state is marked;
            then, the object is used to evaluate the expression
            (that is, *this is scheduled to be evaluated
            within that SchemeContinuation object, and the evaluation
            is launched to run until the marked state is reached again).
            \warning In most cases this leaves the continuation
            exactly in the same state as it was in before the call;
            this is not guaranteed though.
         */
    SchReference Evaluate(class SchemeContinuation &cont) const; 

        //! Apply the expression to the list of arguments
        /*! This method creates a local object of LispContinuation
            and uses it to apply the expression, which must be a
            function, to the given arguments.
         */
    SchReference Apply(const SReference &args) const;

        //! Apply within the given continuation
        /*! This method applies the object, which must be a function,
            to the given arguments within the given continuation,
            just like the respective Evaluate() method does.
         */
    SchReference Apply(const SReference &args,
                       class SchemeContinuation &within) const; 

        //! Is the object 'true' in the sence of Scheme
        /*! Returns false in case the object stores the same
            pointer as PTheSchemeBooleanFalse variable does;
            otherwise, returns true.
         */
    bool IsTrue() const; 

        //! Add another element to list
    SchReference& operator,(const SReference& ref)
        { SReference::operator,(ref); return *this; }

        //! Set the cdr of the last pair of list
    SchReference& operator||(const SReference& ref)
        { SReference::operator||(ref); return *this; }

        //! Make the list (QUOTE *this)
    SchReference operator~() const;

        //! Is the object EQL (in the sence of CL) to the other
    bool IsEql(const SReference& other) const;
        //! Is the object EQUAL (in the sence of CL) to the other
    bool IsEqual(const SReference& other) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
        /*! This version differs from SExpression::TextRepresentation()
            in that it handles the QUOTE form 
         */
    SString TextRepresentation() const;
#endif
};


//! Scheme List constructor
/*! This class is similar to SListConstructor, and is used to create Scheme
    lists. The difference is that
      - this class' operators return SchReference instead of SReference 
      - this class has the operator~() which represents the Scheme QUOTE
        abbreviation (actually, this is used to represent empty lists, 
        like ` ~(L) ').
    \note Altough it looks senseless to derive this class from
        SListConstructor (as it doesn't use anything from it's parent),
        the derivation makes some sence because SListConstructor object is 
        widely used as a representation of empty list (e.g., there's the
        appropriate SReference class constructor)
*/
class SchListConstructor : public SListConstructor {
public:
    SchListConstructor() {}
    ~SchListConstructor() {}
    
    SchReference operator |(const SchReference &t) const
        { return SchReference(new SExpressionCons(t, *PTheEmptyList)); } 
    SchReference operator ^(const SchReference &other) const
        { return SchReference(new SExpressionCons(*PTheEmptyList, other)); }

    SchReference operator ~() const { return *PTheEmptyList; }
};

    //! Pointer to the QUOTE symbol
extern SReference *PTheSchemeSymbolQuote;
    //! Pointer to the #T object
extern SReference *PTheSchemeBooleanTrue;
    //! Pointer to the #F object
extern SReference *PTheSchemeBooleanFalse;

inline bool SchReference::IsTrue() const 
{ return GetPtr() != PTheSchemeBooleanFalse->GetPtr(); } 

inline SchReference SchReference::operator~() const 
{ 
    return 
        SchReference(
            new SExpressionCons(
                *PTheSchemeSymbolQuote, 
                SReference(
                    new SExpressionCons(*this, *PTheEmptyList)
                )
            )
        ); 
}

//! Scheme quotation
/*! As usual, operator~() is used in InteLib to represent Lisp apostrophe.
    This particular version is created for the case we try to quote an
    expression represented with SReference object, which has no operator~()
 */
inline SchReference operator~(const SReference& r)
{ return ~SchReference(r); }



#endif
