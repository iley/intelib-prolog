//   InteLib                                    http://www.intelib.org
//   The file sexpress/slocatn.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*! \file slocatn.hpp
    \brief An assignable location

    This file invents an assignable location (that is, 
    a pointer to an SReference object) represented by
    an S-expression.  See the SExpressionLocation class
    description.
          
*/

#ifndef INTELIB_SLOCATN_HPP_SENTRY
#define INTELIB_SLOCATN_HPP_SENTRY

#include "sexpress.hpp"
#include "iexcept.hpp"

//! assignable location
/*! The class keeps a pointer to an existing SReference object.
    To make sure it will exist as long as the SExpressionLocation
    exists, the object also keeps an SReference to the structure
    which contains the pointed SReference.  For example, if you
    wish to keep a pointer to a car part of a cons, you should 
    also pass a reference to that cons.
 */ 
class SExpressionLocation : public SExpression {
    SReference superstructure;
    SReference *location_pointer;
public:
        //! The type identifier
    static IntelibTypeId TypeId;

        //! The constructor
        /*! \param superstruct is a reference to the data structure
            which contains the location, e.g., to the cons if the
            location is its car or cdr, to the symbol if the location
            is the symbol's dynamic value slot, to the hash table in
            case the location is a pointer to its slot, etc
            \param location is the location to be kept as the object 
         */
    SExpressionLocation(const SReference &superstruct, SReference *location); 

protected:
    ~SExpressionLocation();

public:
        //! Assign the given value to the location
    void Assign(const SReference &value) const { *location_pointer = value; }
    
#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual class SString TextRepresentation() const;
#endif
};    

//! Not an assignable location (that is, SExpressionLocation object)
class IntelibX_not_a_location : public IntelibX {
public:
    IntelibX_not_a_location(SReference a_param);
};

#endif  // sentry
