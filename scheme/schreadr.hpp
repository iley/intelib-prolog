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




/*! \file schreadr.hpp
    \brief The Scheme Reader

    This file provides a reader class tuned to work with Scheme source code,
    not just abstract SExpressions.  See the SchemeReader class description
    for details.
 */
#ifndef INTELIB_SCHREADR_HPP_SENTRY
#define INTELIB_SCHREADR_HPP_SENTRY

#include "../tools/sreader.hpp"

//! The Scheme Reader
/*! This class is an IntelibReader tuned to work with Scheme source code.
    This means it handles:
      - apostrophe, the quote sign
      - the true and false tokens (#T and #F)
      - hash tables and vectors
      - unreadable tokens in the form #<-some-text-here-> as invalid
        lexems
 */
class SchemeReader : public IntelibReader {
public:
    SchemeReader();
};

#endif
