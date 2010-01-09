//   InteLib                                    http://www.intelib.org
//   The file scheme/schxvar.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*! \file schxvar.hpp
    \brief Access global C++ variables from Scheme

    This file provides the SchExtvarConstructor class.
*/

#ifndef INTELIB_SCHXVAR_HPP_SENTRY
#define INTELIB_SCHXVAR_HPP_SENTRY

#include "../genlisp/extvar.hpp"
#include "schsymb.hpp"

//! Scheme external variable expression constructor
/*! This class invents the operator&(), which forms a pair of
    SExpressionIndirectAccessor and SExpressionIndirect.

    The operator takes a C++ lvalue of either any integer, float,
    or SString or SReference type
 */
class SchExtvarConstructor {
public:
    template <class Tt> SReference operator&(Tt &var)
    {
        return
            new SExpressionExternVariable(
                new SExpressionIndirectTemplate<Tt>(var));
    }
};


#endif
