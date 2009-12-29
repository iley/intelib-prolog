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
