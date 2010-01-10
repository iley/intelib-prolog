//  InteLib                                    http://www.intelib.org
//  The file win_port/win_port.hpp
//
//  Copyright (c) Denis Klychkov, 2010
//
//  This is free software, licensed under GNU LGPL v.2.1
//  See the file COPYING for further details.
//
//  THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//  Please see the file WARRANTY for the detailed explanation.

#ifndef INTELIB_WIN_PORT_HPP_SENTRY
#define INTELIB_WIN_PORT_HPP_SENTRY

#ifdef _WIN32

#ifndef pid_t
typedef int pid_t;
#endif

pid_t getppid();

#endif

int execute_cmd_lp(const char *file, char * const arg0, ...);
int execute_cmd_vp(const char *file, char * const argv[]);

#endif
