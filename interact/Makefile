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

none: FORCE
	@echo No default rule

all:	$(TARGETDIRFP)/$(TARGETLIBNAME) FORCE

$(TARGETDIRFP)/$(TARGETLIBNAME):        $(OBJFILES) 
	$(AR) crs $@ $(OBJFILES) 

all_add:        $(OBJFILES) FORCE
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
	$(CXX) -MM $(INTELIB_VERSION) $(LIBSOURCES) > $@


clean: FORCE
	cd $(TARGETDIRFP) && rm -f core *.o a.out *.a \
		test buf gmon.out ii_deps.mk

FORCE:

ifneq (clean, $(MAKECMDGOALS))
-include $(TARGETDIRFP)/ii_deps.mk
endif
