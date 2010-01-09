//   InteLib                                    http://www.intelib.org
//   The file genlisp/extvar.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*! \file extvar.hpp
    \brief A framework to access global C++ variables from Lisp and Scheme

    The stuff defined in this module is to provide an access to global
    C++ variables from within your Lisp or Scheme code.

    There are certain operators (different for Lisp and Scheme; for
    Lisp, it is operator&(LFunctionConstructor,T), while in Scheme we
    define a separate class named SchExtvarConstructor with the operator&(T))
    which take a C++ lvalue of either any integer, float, or SString 
    or SReference type.

    Such operator should form a dotted pair in which the car is a
    SExpressionIndirectAccessor object and the cdr is a SExpressionIndirect
    object which incapsulates the given lvalue.  Being evaluated, such a
    pair returns the current value of the variable; being used as the
    first argument of SETF, it performs the appropriate assignment.
*/

#ifndef INTELIB_EXTVAR_HPP_SENTRY
#define INTELIB_EXTVAR_HPP_SENTRY

#include "../sexpress/sstring.hpp"
#include "conteval.hpp"
#include "lispform.hpp"


//! Abstract indirect variable SETF agent
/*! \internal It's purpose in fact is to provide Value() and Addr() methods */
class SExpressionIndirect : public SExpressionSetfAgent {
public:
        //! Converts whatever variable value to SReference
    virtual SReference Value() const = 0;
        //! Perform SETF on the variable
    virtual void Setf(const SReference &val) = 0;
        //! Get the address of the variable (for TextRepresentation())
    virtual void* Addr() const = 0;
#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif
protected:
    ~SExpressionIndirect() {}
};


//! SETF agent for an extern variable of a particular type
template <class Tt>
class SExpressionIndirectTemplate : public SExpressionIndirect {
    Tt *var;
public:
        //! The constructor
    SExpressionIndirectTemplate(Tt &a) : var(&a) {} 

    virtual SReference Value() const { return SReference(*var); }
    virtual void Setf(const SReference &val);
protected:
    virtual void* Addr() const { return (void*)var; }
    ~SExpressionIndirectTemplate() {}
};


template<> inline
void SExpressionIndirectTemplate<signed char>::Setf(const SReference &val)
{ *var = val.GetInt(); }
template<> inline
void SExpressionIndirectTemplate<unsigned char>::Setf(const SReference &val)
{ *var = val.GetInt(); }
template<> inline
void SExpressionIndirectTemplate<short>::Setf(const SReference &val)
{ *var = val.GetInt(); }
template<> inline
void SExpressionIndirectTemplate<unsigned short>::Setf(const SReference &val)
{ *var = val.GetInt(); }
template<> inline
void SExpressionIndirectTemplate<int>::Setf(const SReference &val)
{ *var = val.GetInt(); }
template<> inline
void SExpressionIndirectTemplate<unsigned int>::Setf(const SReference &val)
{ *var = val.GetInt(); }
template<> inline
void SExpressionIndirectTemplate<long>::Setf(const SReference &val)
{ *var = val.GetInt(); }
template<> inline
void SExpressionIndirectTemplate<unsigned long>::Setf(const SReference &val)
{ *var = val.GetInt(); }
template<> inline
void SExpressionIndirectTemplate<long long>::Setf(const SReference &val)
{ *var = val.GetInt(); }
template<> inline void
SExpressionIndirectTemplate<unsigned long long>::Setf(const SReference &val)
{ *var = val.GetInt(); }


template<> inline
void SExpressionIndirectTemplate<float>::Setf(const SReference &val)
{ *var = val.GetFloat(); }
template<> inline
void SExpressionIndirectTemplate<double>::Setf(const SReference &val)
{ *var = val.GetFloat(); }
template<> inline
void SExpressionIndirectTemplate<long double>::Setf(const SReference &val)
{ *var = val.GetFloat(); }

template<> inline
void SExpressionIndirectTemplate<SString>::Setf(const SReference &val)
{ *var = val.GetString(); }

template<> inline
void SExpressionIndirectTemplate<SReference>::Setf(const SReference &val)
{ *var = val; }




#if 0

//! External variable accessor
/*! This is a very special form (both for Lisp and Scheme)
    which should be called with the SExpressionIndirect
    object INSTEAD of the arguments list, that is, the
    call must be a dotted pair with car being SExpressionIndirectAccessor
    and cdr being SExpressionIndirect.  Don't care much about all
    this stuff.
 */
class SExpressionIndirectAccessor : public SExpressionForm {
public:
    virtual void Call(const SReference &formtail,
                      IntelibContinuation &cont) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const 
    { return "#<-INDIRECT-ACCESSOR->"; }
#endif

protected:
    ~SExpressionIndirectAccessor() {} 
};

#endif

  
class SExpressionExternVariable : public SExpressionSpecialToken {
    SReference indirect;
public:
    static IntelibTypeId TypeId;

    SExpressionExternVariable(SExpressionIndirect *p);
    virtual void Evaluate(class IntelibContinuation& lf); 
protected:
    ~SExpressionExternVariable() {}
};




#endif
