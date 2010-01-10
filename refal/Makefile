#   InteLib                                    http://www.intelib.org
#   The file refal/Makefile
# 
#   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
#   Portions copyright (c) Denis Klychkov, 2010
# 
# 
#   This is free software, licensed under GNU LGPL v.2.1
#   See the file COPYING for further details.
# 
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.




SHELL = /bin/sh

TARGETDIR = ../build

ifneq ($(TARGETDIR),$(filter /%,$(TARGETDIR)))
TARGETDIRFP = $(CURDIR)/$(TARGETDIR)/intelib
else
TARGETDIRFP = $(TARGETDIR)/intelib
endif



TARGETLIBNAME = libirefal.a
LIBSOURCES = refal.cpp rclause.cpp rvars.cpp rfmatch.cpp


LIBFUNSOURCES = $(notdir $(wildcard library/*.cpp))

-include $(TARGETDIRFP)/defines.mk


ifeq ($(OSTYPE),MinGW-win)
WIN_PORT = $(TARGETDIRFP)/win_port.o
endif



OPTIMIZATION = -O2
PROFILING =

COMPILEFLAGS = $(OPTIMIZATION) $(PROFILING) -Wall -Woverloaded-virtual \
		-Wsynth -ggdb $(CONDDEFINES)
COMPILEFLAGS_GENFLAGS = $(OPTIMIZATION) $(PROFILING) -Wall \
		-Wno-parentheses -Woverloaded-virtual -Wsynth \
		-ggdb $(CONDDEFINES)
CXXFLAGS = $(COMPILEFLAGS) -I../sexpress
CXXFLAGS_GENFLAGS = $(COMPILEFLAGS_GENFLAGS) -I../sexpress

OBJFILES = $(patsubst %,$(TARGETDIRFP)/%,$(LIBSOURCES:.cpp=.o))

FUNOBJFILES = $(patsubst %,$(TARGETDIRFP)/rfun_%,$(LIBFUNSOURCES:.cpp=.o))

GEN_DEPSMK = ../gen_deps_mk.sh

DEPSMK = $(TARGETDIRFP)/rf_deps.mk



all: $(TARGETDIRFP)/$(TARGETLIBNAME)

$(TARGETDIRFP)/$(TARGETLIBNAME):  $(WIN_PORT) $(OBJFILES) $(FUNOBJFILES)
	$(AR) crs $@ $(OBJFILES) $(FUNOBJFILES)

all_add: $(WIN_PORT) $(OBJFILES) $(FUNOBJFILES)
	$(AR) rs $(TARGETDIRFP)/$(TARGETLIBNAME) $(OBJFILES) \
	$(FUNOBJFILES) $(WIN_PORT)


$(TARGETDIRFP)/%.o:		%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(WIN_PORT):
ifeq ($(OSTYPE), MinGW-win)
	cd .. && $(MAKE) win_port
endif

$(TARGETDIRFP)/rfun_%.o:	library/%.cpp
	$(CXX) $(CXXFLAGS_GENFLAGS) -c $< -o $@

$(DEPSMK):
	echo > $@
	$(GEN_DEPSMK) --cxx "$(CXX)" \
		--cxxflags '-I../sexpress' \
		--prefix "$(TARGETDIRFP)/" \
		--files "$(LIBSOURCES)" \
		--deps-mk "$@"
	cd library && $(GEN_DEPSMK) --cxx "$(CXX)" \
		--cxxflags '$(CXXFLAGS_GENFLAGS)' \
		--prefix "$(TARGETDIRFP)/rfun_" \
		--files "$(LIBFUNSOURCES)" \
		--deps-mk "$@"

clean:	
	cd $(TARGETDIRFP) && rm -f core *.o *.a gmon.out rf_deps.mk 

FORCE:

ifneq (clean, $(MAKECMDGOALS))
-include $(DEPSMK)
endif
