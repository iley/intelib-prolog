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
