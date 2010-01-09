#   InteLib                                    http://www.intelib.org
#   The file lisp/Makefile
# 
#   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
# 
# 
#   This is free software, licensed under GNU LGPL v.2.1
#   See the file COPYING for further details.
# 
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.

TARGETLIBNAME = libilisp.a

LIBSOURCES = $(wildcard *.cpp)

GENERATED_PREFIX = lfun

MODULES = std sel str io rdr hsh m

GENERATED_FILES = $(MODULES:=.lsp)

include ../lib.mk
