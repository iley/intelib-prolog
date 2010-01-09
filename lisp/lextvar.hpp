//   InteLib                                    http://www.intelib.org
//   The file lisp/lextvar.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*! \file lextvar.hpp
    \brief Access global C++ variables from Lisp

    This file defines the operator&(LFunctionConstructor,T) 

*/

#ifndef INTELIB_LEXTVAR_HPP_SENTRY
#define INTELIB_LEXTVAR_HPP_SENTRY

#include "../genlisp/extvar.hpp"

class LFunctionConstructor;

//! Lisp external variable expression constructor
/*! Operator to form a pair of SExpressionIndirectAccessor
    and SExpressionIndirect, to be used with LFunctionConstructor.
    The operator takes a C++
    lvalue of either any integer, float, or SString or SReference type.
 */
template <class Tt>
SReference operator&(const LFunctionConstructor&, Tt &var)
{
    return SReference(
        new SExpressionExternVariable(
            new SExpressionIndirectTemplate<Tt>(var)));
}

#endif
