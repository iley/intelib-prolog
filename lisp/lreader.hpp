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




/*! \file lreader.hpp
    \brief The Lisp Reader

    This file provides a reader class tuned to work with Lisp source code,
    not just abstract SExpressions.  See the LispReader class description
    for details.
 */
#ifndef INTELIB_LREADER_HPP_SENTRY
#define INTELIB_LREADER_HPP_SENTRY

#include "../tools/sreader.hpp"

//! The Lisp Reader
/*! This class is an IntelibReader tuned to work with Lisp source code.
    This means it handles:
      - apostrophe, the quote sign
      - the token #' which abbreviates FUNCTION
      - the token #& which in InteLib Lisp denotes an external
        C++ variable (only for the code to be translated with ILL)
      - hash tables and vectors
      - unreadable tokens in the form #<-some-text-here-> as invalid
        lexems
 */
class LispReader : public IntelibReader {
public:
    LispReader();
};

//! In InteLib Lisp, #&.... is handled in a special way
class SExpressionExtvarname : public SExpressionString {
public:
        //! The type identifier
    static IntelibTypeId TypeId;
        //! The constructor
    SExpressionExtvarname(const char *s) : SExpressionString(TypeId, s) {}
protected:
    ~SExpressionExtvarname() {}
public:
#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif
};

#endif
