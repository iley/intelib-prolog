#   InteLib                                    http://www.intelib.org
#   The file tools/Makefile
# 
#   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
#   Portions copyright (c) Vasiliy Kulikov, 2010
# 
# 
#   This is free software, licensed under GNU LGPL v.2.1
#   See the file COPYING for further details.
# 
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.




SHELL = /bin/sh

TARGETDIR = $(CURDIR)

ifneq ($(TARGETDIR),$(filter /%,$(TARGETDIR)))
TARGETDIRFP = $(CURDIR)/$(TARGETDIR)/intelib
else
TARGETDIRFP = $(TARGETDIR)/intelib
endif

TARGETLIBNAME = libintelibtools.a

-include $(TARGETDIRFP)/defines.mk

OPTIMIZATION = -O2

COMPILEFLAGS = $(OPTIMIZATION) -Wall -Woverloaded-virtual -Wsynth \
		-ggdb -fpic $(CONDDEFINES) 
CXXFLAGS = $(COMPILEFLAGS) 

LIBSOURCES = $(wildcard *.cpp)

OBJFILES = $(patsubst %,$(TARGETDIRFP)/%,$(LIBSOURCES:.cpp=.o))

DEPSMK = $(TARGETDIRFP)/t_deps.mk

GEN_DEPSMK = ../gen_deps_mk.sh



none: FORCE
	@echo No default rule

all: FORCE $(OBJFILES)
	$(AR) crs $(TARGETDIRFP)/$(TARGETLIBNAME) $(OBJFILES)

all_add: FORCE $(OBJFILES)
	$(AR) rs $(TARGETDIRFP)/$(TARGETLIBNAME) $(OBJFILES)

$(TARGETDIRFP)/%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DEPSMK): Makefile
	echo > $@
	$(GEN_DEPSMK) --cxx "$(CXX)" \
		--cxxflags "$(CXXFLAGS)" \
		--prefix "$(TARGETDIRFP)/" \
		--files "$(LIBSOURCES)" \
		--deps-mk "$@"

clean: FORCE
	cd $(TARGETDIRFP) && rm -f core *.o a.out *.a \
		test buf gmon.out t_deps.mk

FORCE:

ifneq (clean, $(MAKECMDGOALS))
-include $(DEPSMK)
endif
