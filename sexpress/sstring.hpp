//   InteLib                                    http://www.intelib.org
//   The file sexpress/sstring.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*!
  \file sstring.hpp
  \brief SString class definition

    The file defines the SString class which may be used instead of 
    the standard string class when the strings are not to change. 
    See the SString class description for details.
 */

#ifndef INTELIB_SSTRING_HPP_SENTRY
#define INTELIB_SSTRING_HPP_SENTRY

#include "sexpress.hpp"
#include "gensref.hpp"


//! Just a generic smart pointer to a SExpressionString
typedef GenericSReference<SExpressionString, IntelibX_not_a_string>
SStringRef;

//! A container for a string
/*! SString class implements a string as an SReference to an
    SExpressionString object. Some useful operations are defined so that
    one can use the class instead of the standard string class. 
    \note the underlyng SExpressionString object is never changed.
    All the changes are implemented releazing the old object and
    creating a new one.  This means the underlying strings can be
    shared without any doubt.
*/
class SString : public SStringRef {
public:
        //! Make empty string
    SString() : SStringRef(new SExpressionString("")) {}
        //! Make a string from a string literal
    SString(const char *s) : SStringRef(new SExpressionString(s)) {}
        //! Hold the given object
    SString(SExpressionString *ts) : SStringRef(ts) {}
        //! The copy constructor
    SString(const SString &other) : SStringRef(other) {}
        //! Type-checking construction from a reference
    SString(const SReference &other) : SStringRef(other) {}
        //! The destructor
    ~SString() {}

        //! Assign
    SString& operator =(const char *s);

        //! Concatenation
    SString operator+(const SString &other) const;
        //! Append another string
    void operator+=(const SString &other);
        //! Concatenation
    SString operator+(const char *s) const;
        //! Append another string
    void operator+=(const char *s);
        //! Append a character
    void operator+=(char c);
        //! Get a plain pointer to the string
    const char *c_str() const;
        //! Get the element (char) by index
        /*! \warning no range checking is performed */
    char operator[](int index) const { return c_str()[index]; }
        //! Get the length of the string
    unsigned int length() const;
        //! Get the length of the string
    unsigned int Length() const { return length(); }
    
        //! String comparision
    bool operator==(const SString& other) const;
        //! String comparision
    bool operator!=(const SString& other) const;
};


#endif
