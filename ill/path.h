//  InteLib                                    http://www.intelib.org
//  The file ill/path.h
//
//  Copyright (c) Denis Klychkov, 2010
//
//
//  This is free software, licensed under GNU LGPL v.2.1
//  See the file COPYING for further details.
//
//  THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//  Please see the file WARRANTY for the detailed explanation.

#ifndef PATH_H
#define PATH_H

#ifdef _WIN32

/* Windows-type separators */
const char pathseparator = ';';
const char dirseparator = '\\';

#else

/* UNIX-type separators */
const char pathseparator = ':';
const char dirseparator = '/';

#endif

#endif
