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
