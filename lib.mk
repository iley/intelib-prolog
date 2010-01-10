#   InteLib                                    http://www.intelib.org
#   The file lib.mk
# 
#   Copyright (c) Vasiliy Kulikov (<segooon_AT_gmail.com>), 2010
# 
#   This is free software, licensed under GNU LGPL v.2
#   See the file COPYING for further details.
# 
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.


# This file is included from
#         $intelib/{lisp,genlisp,scheme,sexpress,tools}/Makefile
#
# To include this .mk from your Makefile, you need to define variables:
#
# TARGETLIBNAME = name of .a library
#
# LIBSOURCES = $(wildcard *.cpp) is a good idea
#
# GENERATED_PREFIX = lfun or another prefix for generated .hpp files
#
# MODULES = all directories in library/
#
# GENERATED_FILES = $(MODULES:=.lsp)


SHELL = /bin/sh

TARGETDIR = $(CURDIR)

ifneq ($(TARGETDIR),$(filter /%,$(TARGETDIR)))
TARGETDIRFP = $(CURDIR)/$(TARGETDIR)/intelib
else
TARGETDIRFP = $(TARGETDIR)/intelib
endif

-include $(TARGETDIRFP)/defines.mk

OPTIMIZATION = -O2

COMPILEFLAGS = $(OPTIMIZATION) -Wall -Woverloaded-virtual -Wsynth \
		-ggdb -fpic $(CONDDEFINES) 
CXXFLAGS = $(COMPILEFLAGS) 

OBJFILES = $(patsubst %,$(TARGETDIRFP)/%,$(LIBSOURCES:.cpp=.o))

DEPSMK = $(TARGETDIRFP)/$(GENERATED_PREFIX)_deps.mk

GEN_DEPSMK = ../gen_deps_mk.sh



none:
	@echo No default rule

all:	$(TARGETDIRFP)/$(TARGETLIBNAME)

$(TARGETDIRFP)/$(TARGETLIBNAME):        $(OBJFILES) library/ALL
	$(AR) crs $@ $(OBJFILES) $(TARGETDIRFP)/$(GENERATED_PREFIX)_*.o

all_add:        $(OBJFILES) library/ALL
	$(AR) rs $(TARGETDIRFP)/$(TARGETLIBNAME)\
		$(OBJFILES) $(TARGETDIRFP)/$(GENERATED_PREFIX)_*.o


$(TARGETDIRFP)/%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DEPSMK): Makefile
	echo > $@
	$(GEN_DEPSMK) --cxx "$(CXX)" \
		--cxxflags "$(CXXFLAGS)" \
		--prefix "$(TARGETDIRFP)/" \
		--files "$(LIBSOURCES)" \
		--deps-mk "$@"

library/ALL: FORCE
	cd library && $(MAKE) all TARGETDIRFP=$(TARGETDIRFP) \
  			CXXFLAGS="$(CXXFLAGS)" \
  			MODULES="$(MODULES)"
clean:	
	cd $(TARGETDIRFP) && rm -f core *.o a.out *.a \
		test buf gmon.out *deps.mk $(GENERATED_PREFIX)_*.hpp \
		*.lsp \
		*.scm

FORCE:

ifneq (clean, $(MAKECMDGOALS))
-include $(DEPSMK)
endif
