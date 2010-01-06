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
