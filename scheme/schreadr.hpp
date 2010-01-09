//   InteLib                                    http://www.intelib.org
//   The file scheme/schreadr.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




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
