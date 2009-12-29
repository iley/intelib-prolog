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




/*!
  \file iexcept.hpp
  \brief Intelib exceptions 
  
  This file defines some classes used in the exception handling 
  by InteLib.
*/

#ifndef INTELIB_IEXCEPT_HPP_SENTRY
#define INTELIB_IEXCEPT_HPP_SENTRY

#include "sexpress.hpp"

//! The generic InteLib exception
/*! The IntelibX class is the superclass of all existing exceptional 
    situations detected/generated within the library. It encapsulates 
    the functionality necessary to hold the description of a particular
    exceptions category (as a C string), the parameter which describes 
    a particular exception (as an S-expression) and the Lisp calling 
    stack for the moment exception had been thrown (as an S-expression). 
     \par The children of IntelibX (such as IntelibX_not_a_string, 
    IntelibX_cant_evaluate etc.) are used in actual exception handling.  
     \note the class has no virtual table.
*/
class IntelibX {
       //! The exception's category description
       /*! This parameter is usually set by the child's  
           constructor in order to describe the exception's 
           category (such as "Not a string", "Can not evaluate" etc.)
        */
     const char *descr; 
       //! The parameter thrown together with the exception
       /*! This is used to hold the S-expression passed to the 
           constructor of a particular exception to describe what 
           was actually wrong (e.g. the S-expression which couldn't 
           be evaluated or wasn't a string)
        */
     SReference param;  //!< The S-expression holding 

       //! The incrementable stack 
       /*! This is used to hold an execution stack in case of 
           an evaluation environment (such as Lisp machine)        
        */
     SReference stack;  
public:
       //! The 1st form of constructor 
       /*! Constructs an exception without a parameter, 
           such as IntelibX_unexpected_unbound_value.
           It's assumed there's no relevant S-expression.
        */
     IntelibX(const char *a_descr);
       //! The 1st form of constructor 
       /*! Constructs an exception with a parameter, 
           which is assumed to be somehow relevant to the
           problem detected (such as the S-expression which
           fails to meet some requirements)
        */
     IntelibX(const char *a_descr, SReference a_param);
       //! The copy constructor
       /*! Strings are copied. SReferences are assigned.
        */
     IntelibX(const IntelibX& other);
       //! The destructor
     ~IntelibX(); 

       //! Add an element to the carried stack-related information
     void AddStack(SReference r);

       //! Get the description
     const char *Description() const { return descr; }
       //! Get the parameter
     const SReference &Parameter() const { return param; }
       //! Get the stack
     const SReference &Stack() const { return stack; }
};



//! Wrong expression type 
class IntelibX_wrong_expression_type : public IntelibX {
public:
    IntelibX_wrong_expression_type(const SReference &a_param);
};

//! String expected
class IntelibX_not_a_string : public IntelibX {
public:
    IntelibX_not_a_string(SReference a_param);
};

//! Number expected
class IntelibX_not_a_number : public IntelibX {
public:
    IntelibX_not_a_number(SReference a_param);
};

//! Single char expected
class IntelibX_not_a_char : public IntelibX {
public:
    IntelibX_not_a_char(SReference a_param);
};

//! Cons expected
class IntelibX_not_a_cons : public IntelibX {
public:
    IntelibX_not_a_cons(SReference a_param);
};

//! List expected
class IntelibX_not_a_list : public IntelibX {
public:
    IntelibX_not_a_list(SReference a_param);
};

//! Label expected
class IntelibX_not_a_label : public IntelibX {
public:
    IntelibX_not_a_label(SReference a_param);
};

//! Unexpected NULL (unbound) SReference encountered
class IntelibX_unexpected_unbound_value : public IntelibX {
public:
    IntelibX_unexpected_unbound_value();
};

//! Feature not implemented yet
class IntelibX_not_implemented : public IntelibX {
public:
    IntelibX_not_implemented();
};

//! Circumstances are wrong so that it is definitely a bug in the library
/*! \note PLEASE contact the developers if you get this exception!
 */
class IntelibX_bug : public IntelibX {
public:
    IntelibX_bug();
};



#if INTELIB_RUNTIME_CHECKS == 1
    //! Assertion macro which can be disabled for a production version
# define INTELIB_ASSERT(condition, exception) \
	do { if(!(condition)) throw exception; } while(0)
#else
    //! Disabled version of the assertion macro
# define INTELIB_ASSERT(condition, exception) 
#endif

#endif // sentry
