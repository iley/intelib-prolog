#   InteLib                                    http://www.intelib.org
#   The file interact/Makefile
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

none:
	@echo No default rule

all:	$(TARGETDIRFP)/$(TARGETLIBNAME)

$(TARGETDIRFP)/$(TARGETLIBNAME):        $(OBJFILES) 
	$(AR) crs $@ $(OBJFILES) 

all_add:        $(OBJFILES) 
	$(AR) rs $(TARGETDIRFP)/$(TARGETLIBNAME) $(OBJFILES) 

$(TARGETDIRFP)/%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGETDIRFP)/ireadln.o: ireadln.cpp $(TARGETDIRFP)/.check_readline\
	../sexpress/sexpress.hpp ../tools/sreader.hpp ../tools/sstream.hpp

$(TARGETDIRFP)/.check_readline:
ifeq ($(USE_READLINE),yes)
	@echo '#include<stdio.h>'              > _cr.c
	@echo '#include <readline/readline.h>' >> _cr.c
	@$(CXX) -c _cr.c -o /dev/null > /dev/null 2>&1 || \
		(echo "NO GNU READLINE FOUND.";\
		echo "INSTALL IT OR TRY";\
		echo "             $(MAKE) [<your target>] USE_READLINE=no";\
		exit 1)
	rm -f _cr.c || :
else 
	@:
endif
	touch $@

$(TARGETDIRFP)/ii_deps.mk: $(LIBSOURCES) Makefile
	$(CXX) -MM $(INTELIB_VERSION) $(LIBSOURCES) \
	| sed '/^[^ ]/s/^/$(subst /,\/,$(TARGETDIRFP))\//g' > $@


clean:	
	cd $(TARGETDIRFP) && rm -f core *.o a.out *.a \
		test buf gmon.out ii_deps.mk

FORCE:

ifneq (clean, $(MAKECMDGOALS))
-include $(TARGETDIRFP)/ii_deps.mk
endif
