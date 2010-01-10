#   InteLib                                    http://www.intelib.org
#   The file interact/Makefile
# 
#   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
#   Portions copyright (c) Vasiliy Kulikov, 2010
#   Portions copyright (c) Denis Klychkov, 2010
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

TARGETLIBNAME = libintelibinteract.a

# change this to "no" if you don't have GNU readline development files
USE_READLINE=yes

ifeq ($(USE_READLINE),yes)
    USEREADLINEFLAG = -DINTELIB_USE_READLINE=1
else 
    USEREADLINEFLAG = -DINTELIB_USE_READLINE=0
endif

-include $(TARGETDIRFP)/defines.mk

OPTIMIZATION = -O2

COMPILEFLAGS = $(OPTIMIZATION) -Wall -Woverloaded-virtual -Wsynth \
		-ggdb -fpic $(CONDDEFINES) $(USEREADLINEFLAG)
CXXFLAGS = $(COMPILEFLAGS) 

LIBSOURCES = ireadln.cpp

OBJFILES = $(patsubst %,$(TARGETDIRFP)/%,$(LIBSOURCES:.cpp=.o))

DEPSMK = $(TARGETDIRFP)/ii_deps.mk

GEN_DEPSMK = ../gen_deps_mk.sh



none: FORCE
	@echo No default rule

all:	$(TARGETDIRFP)/$(TARGETLIBNAME) FORCE

$(TARGETDIRFP)/$(TARGETLIBNAME):        $(OBJFILES) 
	$(AR) crs $@ $(OBJFILES) 

all_add:        $(OBJFILES) FORCE
	$(AR) rs $(TARGETDIRFP)/$(TARGETLIBNAME) $(OBJFILES) 

$(TARGETDIRFP)/ireadln.o: ireadln.cpp .check_readline
	$(CXX) $(CXXFLAGS) -c $< -o $@

.check_readline:
ifeq ($(USE_READLINE),yes)
	CXX=$(CXX) MAKE=$(MAKE) ./check_readline.sh
else 
	@:
endif

$(DEPSMK): Makefile
	echo > $@
	$(GEN_DEPSMK) --cxx "$(CXX)" \
		--cxxflags "$(CXXFLAGS)" \
		--output "$(TARGETDIRFP)/ireadln.o" \
		--files "$(LIBSOURCES)" \
		--deps-mk "$@"

clean: FORCE
	cd $(TARGETDIRFP) && rm -f core *.o a.out *.a \
		test buf gmon.out ii_deps.mk

FORCE:

ifneq (clean, $(MAKECMDGOALS))
-include $(DEPSMK)
endif
