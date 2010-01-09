//   InteLib                                    http://www.intelib.org
//   The file tools/prprint.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*! \file prprint.hpp
    \brief pretty printing of an S-expression

    See the pretty_print function decription
 */

#ifndef INTELIB_PRPRINT_HPP_SENTRY
#define INTELIB_PRPRINT_HPP_SENTRY



typedef bool (*prettyprint_callback_function)(const char*, void *);

//! Pretty print an S-expression
/*! Lists are printed on one line if they fit the line,
    otherwise they get printed one item per line, indented.

    \param ref is the expression to print
    \param fun is the function which is capable of printing strings;
           actually, it can do anything with the string, e.g. printing,
           storing, converting to graphics etc.
    \param userdata is arbitrary data to pass to the callback function,
           such as a stream (FILE*) or whatever you need
    \param indent is the starting indentation
    \param margin is the right margin
    \param indentstep is how many spaces to use to indent inner blocks
 */
bool pretty_print(SReference ref, 
                  prettyprint_callback_function fun, 
                  void *userdata, 
                  int indent = 0, int margin = 75, int indentstep = 4);

#endif
