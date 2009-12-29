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




/*! \file gensref.hpp
    \brief Generic type-specific subclass of SReference

    This file defines the GenericSReference template which is
    to be used to define SReference's children intended to handle
    SExpression hierarchy member objects of a particular type.
 */

#ifndef INTELIB_GENSREF_HPP_SENTRY
#define INTELIB_GENSREF_HPP_SENTRY

#include "iexcept.hpp"
#include "sexpress.hpp"

//! Generic subclass of SReference for a particular S-expression type
/*! \param Tp is the S-expression type
    \param X_wrongtype is the type of exception to throw on a type
           mismatch
 */
template<class Tp, class X_wrongtype> 
class GenericSReference : public SReference {
public:
        //! Default constructor (unbound reference)
    GenericSReference() : SReference() {}
        //! Makes a reference to the given object
    GenericSReference(const Tp *ts)
       : SReference((const SExpression*)ts) {}
        //! Copy constructor
    GenericSReference(const GenericSReference<Tp, X_wrongtype> &other)
       : SReference(other) {}
        //! Typecast constructor
        /*! 
            \param other the object to copy
            \param ignore_type specifies whether to skip the type check
            If the check is not set to be skipped and fails, the 
            exception is thrown
         */
    GenericSReference(const SReference &other, bool ignore_type = false) 
       : SReference(other)
    {
        if(!ignore_type && other.GetPtr() &&
           !other->TermType().IsSubtypeOf(Tp::TypeId))
        {
            throw X_wrongtype(other);
        }
    }
    ~GenericSReference() {}
 
        //! Get the appropriately typed pointer to the referenced object
    Tp* GetPtr() const
        { return static_cast<Tp*>(SReference::GetPtr()); }

        //! Dereference (returns a reference to the pointe object)
    Tp& operator*() const { return *GetPtr(); }
        //! Select a field/method of the referenced object
    Tp* operator->() const { return GetPtr(); }
        //! Assignment
    GenericSReference<Tp,X_wrongtype>&
    operator =(const GenericSReference<Tp,X_wrongtype>& ref)
        { SReference::operator=(ref); return *this; }
        //! Assignment
    GenericSReference<Tp,X_wrongtype>& operator =(Tp *lt)
        { SReference::operator=(lt); return *this; }
        //! Assignment
    GenericSReference<Tp,X_wrongtype>& operator =(Tp &lt)
        { SReference::operator=(lt); return *this; }
private:
    // made private to prevent the user from mistakes and
    //   to make sure this always points to an object of Tp.
    void operator=(const SReference& ref) {}
    // made private as senseless
    void operator,(const SReference& ref) {}
    void operator||(const SReference& ref) {}
};


#endif // sentry
