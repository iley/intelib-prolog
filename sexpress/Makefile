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

TARGETLIBNAME = libsexpr.a

-include $(TARGETDIRFP)/defines.mk


../version.h:
	cd .. && $(MAKE) version.h

OPTIMIZATION = -O2

CXXFLAGS = $(OPTIMIZATION) -Wall -Woverloaded-virtual -Wsynth \
		-ggdb -fpic $(CONDDEFINES) 

LIBSOURCES = sexpress.cpp iexcept.cpp \
	sstring.cpp svector.cpp shashtbl.cpp sbacklnk.cpp sdbllist.cpp \
	squeue.cpp srawbuf.cpp slocatn.cpp

OBJFILES = $(patsubst %,$(TARGETDIRFP)/%,$(LIBSOURCES:.cpp=.o))

DEPSMK = $(TARGETDIRFP)/sx_deps.mk

GEN_DEPSMK = ../gen_deps_mk.sh


none:
	@echo No default rule

$(TARGETDIRFP)/%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGETDIRFP)/ireadln.o:	ireadln.cpp
	$(CXX) $(CXXFLAGS) $(USEREADLINEFLAG) -c $< -o $@

$(TARGETDIRFP)/$(TARGETLIBNAME):	$(OBJFILES)
	$(AR) crs $@ $^ 

all:	$(TARGETDIRFP)/$(TARGETLIBNAME)
	:

$(DEPSMK): Makefile
	$(GEN_DEPSMK) --cxx "$(CXX)" \
		--cxxflags "$(CXXFLAGS) -I.." \
		--prefix "$(TARGETDIRFP)/" \
		--files "$(LIBSOURCES)" \
		--deps-mk "$@"

clean:	
	cd $(TARGETDIRFP) && rm -f core *.o a.out *.a \
		test buf gmon.out sx_deps.mk lib.lsp

FORCE:
	@:

ifneq (clean, $(MAKECMDGOALS))
-include $(TARGETDIRFP)/sx_deps.mk
endif
