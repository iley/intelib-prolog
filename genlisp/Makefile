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

TARGETLIBNAME = libintelibgenlisp.a

GENLISPLIBMODULES = std str hsh io rdr m

-include $(TARGETDIRFP)/defines.mk

OPTIMIZATION = -O2

COMPILEFLAGS = $(OPTIMIZATION) -Wall -Woverloaded-virtual -Wsynth \
		-ggdb -fpic $(CONDDEFINES) 
CXXFLAGS = $(COMPILEFLAGS) 

LIBSOURCES = conteval.cpp lispform.cpp binds.cpp extvar.cpp dumpcont.cpp

OBJFILES = $(patsubst %,$(TARGETDIRFP)/%,$(LIBSOURCES:.cpp=.o))

none:
	@echo No default rule

all:	$(TARGETDIRFP)/$(TARGETLIBNAME)

$(TARGETDIRFP)/$(TARGETLIBNAME):        $(OBJFILES) library/ALL
	$(AR) crs $@ $(OBJFILES) $(TARGETDIRFP)/glsp_*.o

all_add:        $(OBJFILES) library/ALL
	$(AR) rs $(TARGETDIRFP)/$(TARGETLIBNAME)\
		$(OBJFILES) $(TARGETDIRFP)/glsp_*.o


$(TARGETDIRFP)/%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGETDIRFP)/gls_deps.mk: $(LIBSOURCES) Makefile
	$(CXX) -MM $(INTELIB_VERSION) $(LIBSOURCES) \
	| sed '/^[^ ]/s/^/$(subst /,\/,$(TARGETDIRFP))\//g' > $@

library/ALL: FORCE
	cd library && $(MAKE) all TARGETDIRFP=$(TARGETDIRFP) \
  			CXXFLAGS="$(COMPILEFLAGS)" \
  			GENLISPLIBMODULES="$(GENLISPLIBMODULES)"

clean:	
	cd $(TARGETDIRFP) && rm -f core *.o a.out *.a \
		test buf gmon.out gls_deps.mk

FORCE:

ifneq (clean, $(MAKECMDGOALS))
-include $(TARGETDIRFP)/gls_deps.mk
endif
