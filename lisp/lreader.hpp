//   InteLib                                    http://www.intelib.org
//   The file lisp/lreader.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




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
