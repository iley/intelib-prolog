# +-------------------------------------------------------------------------+
# |               I__n__t__e__L__i__b           0.6.21 development          |
# | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
# |                                                                         |
# | This is free software. The library part is available under              |
# |                               GNU LESSER GENERAL PUBLIC LICENSE v.2.1.  |
# | GNU LGPL v2.1 is found in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
# |     Please see also docs/readme.txt and visit http://www.intelib.org    |
# |                                                                         |
# | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
# +-------------------------------------------------------------------------+




SHELL = /bin/sh

TARGETDIR = $(CURDIR)

ifneq ($(TARGETDIR),$(filter /%,$(TARGETDIR)))
TARGETDIRFP = $(CURDIR)/$(TARGETDIR)
else
TARGETDIRFP = $(TARGETDIR)
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
	$(GEN_DEPSMK) --cxx "$(CXX)" \
		--cxxflags "$(CXXFLAGS)" \
		--prefix "$(TARGETDIRFP)" \
		--files "$(LIBSOURCES)" \
		--deps-mk "$@"

clean: FORCE
	cd $(TARGETDIRFP) && rm -f core *.o a.out *.a \
		test buf gmon.out ii_deps.mk

FORCE:

ifneq (clean, $(MAKECMDGOALS))
-include $(DEPSMK)
endif
