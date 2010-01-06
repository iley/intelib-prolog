# This file is included in intelib/$1/Makefile
# where $1 is one of lisp/ genlisp/ scheme/ sexpress/ tools/.
#
#
# To include this .mk to your Makefile, you have to define variables:
#
# TARGETLIBNAME = name of .a library
#
# LIBSOURCES = $(wildcard *.cpp) is good idea
#
# GENERATED_PREFIX = lfun or another prefix for generated .hpp files
#
# MODULES = all directories in library/
#
# GENERATED_FILES = $(MODULES:=.lsp)
#

SHELL = /bin/sh

TARGETDIR = $(CURDIR)

ifneq ($(TARGETDIR),$(filter /%,$(TARGETDIR)))
TARGETDIRFP = $(CURDIR)/$(TARGETDIR)
else
TARGETDIRFP = $(TARGETDIR)
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
	$(GEN_DEPSMK) --cxx $(CXX) \
		--cxxflags "$(CXXFLAGS)" \
		--prefix $(TARGETDIRFP) \
		--files "$(LIBSOURCES)" \
		--deps-mk $@

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
