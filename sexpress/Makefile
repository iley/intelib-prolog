#   InteLib                                    http://www.intelib.org
#   The file sexpress/Makefile
# 
#   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
#   Portions copyright (c) Vasiliy Kulikov, 2010
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
	echo > $@
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
