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

TARGETDIR = ../build

ifneq ($(TARGETDIR),$(filter /%,$(TARGETDIR)))
TARGETDIRFP = $(CURDIR)/$(TARGETDIR)
else
TARGETDIRFP = $(TARGETDIR)
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

$(TARGETDIRFP)/rf_deps.mk: $(LIBSOURCES) Makefile
	$(CXX) -MM $(INTELIB_VERSION) -I../sexpress $(LIBSOURCES) \
	| sed '/^[^ ]/s/^/$(subst /,\/,$(TARGETDIRFP))\//g' > $@


clean:	
	cd $(TARGETDIRFP) && rm -f core *.o *.a gmon.out rf_deps.mk 

FORCE:

ifneq (clean, $(MAKECMDGOALS))
-include $(TARGETDIRFP)/rf_deps.mk
endif
