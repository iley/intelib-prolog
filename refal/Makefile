#   InteLib                                    http://www.intelib.org
#   The file refal/Makefile
# 
#   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
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
TARGETDIRFP = $(CURDIR)/$(TARGETDIR)
else
TARGETDIRFP = $(TARGETDIR)
endif



TARGETLIBNAME = libirefal.a
LIBSOURCES = refal.cpp rclause.cpp rvars.cpp rfmatch.cpp


LIBFUNSOURCES = $(notdir $(wildcard library/*.cpp))

-include $(TARGETDIRFP)/defines.mk



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

all:	$(TARGETDIRFP)/$(TARGETLIBNAME)

$(TARGETDIRFP)/$(TARGETLIBNAME):        $(OBJFILES) $(FUNOBJFILES)
	$(AR) crs $@ $(OBJFILES) $(FUNOBJFILES)

all_add:        $(OBJFILES) $(FUNOBJFILES)
	$(AR) rs $(TARGETDIRFP)/$(TARGETLIBNAME) $(OBJFILES) \
	$(FUNOBJFILES)


$(TARGETDIRFP)/%.o:		%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

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
