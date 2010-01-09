#   InteLib                                    http://www.intelib.org
#   The file ils/Makefile
# 
#   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
# 
# 
#   This is free software, licensed under GNU GPL v.2
#   See the file COPYING for further details.
# 
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.




SHELL = /bin/sh

# change this to "no" if you don't want math functions (sin, cos, etc...)
# in the Lisp interpreter
LISPMATH=yes

# change this to "no" if you don't have GNU readline development files
USE_READLINE=yes

# The directory in which the library will be built
TARGETDIR = ../build

ifneq ($(TARGETDIR),$(filter /%,$(TARGETDIR)))
TARGETDIRFP = $(CURDIR)/$(TARGETDIR)/intelib
else
TARGETDIRFP = $(TARGETDIR)/intelib
endif

OSTYPE = $(shell uname -s)

DEPSMK = $(TARGETDIRFP)/ils_deps.mk

GEN_DEPSMK = ../gen_deps_mk.sh


LOCALFLAGS = -fpic -funsigned-char -g -Wall -I$(TARGETDIRFP)/.. -I$(TARGETDIRFP)

SCHEMELIBMODULES = std str sel io rdr hsh m
LIBINTELIB = -L$(TARGETDIRFP) -lintelib -lintelib_interp
LFUNHEADERS = $(patsubst %,$(TARGETDIRFP)/sch_%.hpp,$(SCHEMELIBMODULES))
LIBDEFSCMFILES = $(patsubst %,$(TARGETDIRFP)/%.scm,$(SCHEMELIBMODULES))
LIBDEPEND = $(LFUNHEADERS) $(TARGETDIRFP)/libintelib.a \
			$(TARGETDIRFP)/libintelib_interp.a


ifeq ($(OSTYPE),Linux)
    LIBDL = -ldl
    DYNAMIC = -export-dynamic
    TDIR = $(TARGETDIRFP)
else
ifeq ($(OSTYPE),MinGW-win)
    LIBDL = -ldl
    DYNAMIC = -export-all-symbols
    INCDL = -I../win_port
# This string translates an absolute UNIX-like path to an absolute windows path
# with double backslashes. E.g. /c/inelib -> c:\\intelib
    TDIR = $(shell echo $(TARGETDIRFP) | sed 's/^\/\(.\)/\1:/' | sed 's/\//\\\\/g')
else
    LIBDL = 
    DYNAMIC = -export-dynamic
    TDIR = $(TARGETDIRFP)
endif
endif

# ifeq ($(LISPMATH),yes)
#     LISPLIBMODULES = std sel str io rdr hsh m
#     LIBMATH = -lm
# else 
#     LISPLIBMODULES = std sel str io rdr hsh
#     LIBMATH = 
# endif

ifeq ($(USE_READLINE),yes)
    #LIBREADLINE = -lreadline /lib/libtinfo.so.5
    LIBREADLINE = -lreadline
    USEREADLINEFLAG = -DINTELIB_USE_READLINE=1
else 
    LIBREADLINE = 
    USEREADLINEFLAG = -DINTELIB_USE_READLINE=0
endif


NILS = $(TARGETDIRFP)/nils
ILS= $(TARGETDIRFP)/ils

default:
	@echo "No default rule here."
	@echo "If you only need to get a working binary, the most"
	@echo "appropriate target for you is 'bootstrap'"

CXXFLAGS = $(LOCALFLAGS) 

###############################
# library symbols and other definitions

$(TARGETDIRFP)/ssympkgen:	sympkgen.cpp
	$(CXX) $(CXXFLAGS) $^ $(LIBINTELIB) -o $@

$(TARGETDIRFP)/ssympkg.cxx:	$(TARGETDIRFP)/ssympkgen
	cat $(LIBDEFSCMFILES) ilsbdef.scm | $(TARGETDIRFP)/ssympkgen > $@

$(TARGETDIRFP)/ssympkg.o:	$(TARGETDIRFP)/ssympkg.cxx
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(TARGETDIRFP)/strtgen:		trtgen.c
	$(CXX) $(CXXFLAGS) trtgen.c -o $@

$(TARGETDIRFP)/schartran._sc:	$(TARGETDIRFP)/strtgen
	$(TARGETDIRFP)/strtgen > $@

$(TARGETDIRFP)/ilsdef._sc:	$(TARGETDIRFP)/schartran._sc ilsbdef.scm
	echo '(%%%' > $@
	cat $^ >> $@
	#cat ../build/*.scm >> $@
	cat $(LIBDEFSCMFILES) >> $@
	echo ')' >> $@

###############################
# Interpreter

$(TARGETDIRFP)/ils_loop.o: ils_loop.cpp ils_loop.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

$(TARGETDIRFP)/nils.o: nils.cpp ils_loop.hpp ../version.h
	$(CXX) $(CXXFLAGS) $(INCDL) -c $< -o $@ 

nils: $(TARGETDIRFP)/nils
	:

$(TARGETDIRFP)/nils: $(TARGETDIRFP)/nils.o $(TARGETDIRFP)/ils_loop.o $(TARGETDIRFP)/ssympkg.o
	$(CXX) $(CXXFLAGS) $^  -o $@ \
	  $(LIBINTELIB) $(LIBMATH) $(LIBREADLINE) $(LIBDL) -Wl,$(DYNAMIC)

###############################
# Compiler

###
# build with NILS

$(TARGETDIRFP)/ils_nils.cxx $(TARGETDIRFP)/ils_nils.hxx:	$(NILS)	ils.scm $(TARGETDIRFP)/ilsdef._sc
	cp ils.scm $(TARGETDIRFP)
	cd $(TARGETDIRFP) && \
	$(NILS) -i ils.scm \
		-x '(cmdlinedirectives "(MODULE-NAME \"ils_nils\")")'\
		-x '(ils_main (quote ("ilsdef._sc" "ils.scm")))' -Q

$(TARGETDIRFP)/ils_nils.o:	$(TARGETDIRFP)/ils_nils.cxx $(TARGETDIRFP)/ils_nils.hxx ../version.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

ils_nils: $(TARGETDIRFP)/ils_nils
	:

$(TARGETDIRFP)/ils_nils:	ilsmain.cpp $(TARGETDIRFP)/ils_nils.o $(LIBDEPEND)
	$(CXX) $(CXXFLAGS) -DHXXFILE=\"ils_nils.hxx\" $(TARGETDIRFP)/ils_nils.o \
		ilsmain.cpp -o $@ $(LIBINTELIB)

###
# build with itself


$(TARGETDIRFP)/ils.cxx $(TARGETDIRFP)/ils.hxx:	ils.scm $(TARGETDIRFP)/ilsdef._sc
	cp ils.scm $(TARGETDIRFP)
	cd $(TARGETDIRFP) && \
	$(ILS) ils.scm	

$(TARGETDIRFP)/ils.o:	$(TARGETDIRFP)/ils.cxx $(TARGETDIRFP)/ils.hxx
	$(CXX) $(CXXFLAGS) -c $(TARGETDIRFP)/ils.cxx -o $@


ils_new: $(TARGETDIRFP)/ils_new
	:

$(TARGETDIRFP)/ils_new:	$(TARGETDIRFP)/ils.o ilsmain.cpp $(LIBDEPEND)
	$(CXX) $(CXXFLAGS) $(TARGETDIRFP)/ils.o ilsmain.cpp $(LIBINTELIB) -o $@


$(DEPSMK): Makefile
	echo > $@
	$(GEN_DEPSMK) --cxx "$(CXX)" \
		--cxxflags "$(CXXFLAGS)" \
		--output "$(TARGETDIRFP)/ssympkgen" \
		--files "sympkgen.cpp" \
		--deps-mk "$@"
	$(GEN_DEPSMK) --cxx "$(CXX)" \
		--cxxflags "$(CXXFLAGS)" \
		--prefix "$(TARGETDIRFP)/" \
		--files "ssympkg.cxx ils_loop.cpp" \
		--deps-mk "$@"


###############################
# bootstrap

bootstrap:
	@echo "-------------------------------------------------------------"
	@echo "    Building NILS interpreter                                "
	@echo "-------------------------------------------------------------"
	$(MAKE) nils
	@echo "-------------------------------------------------------------"
	@echo "    Building an ILS binary by self-applying ILS under NILS   "
	@echo "-------------------------------------------------------------"
	$(MAKE) ils_nils
	@echo "-------------------------------------------------------------"
	@echo "    Testing the newly-built binary                           "
	@echo "-------------------------------------------------------------"
	$(TARGETDIRFP)/ils_nils ils.scm \
		'(MODULE-NAME "ils_nils")' \
		'(CXX-FILE-PATH "$(TDIR)/ils_n_t.cxx")' \
		'(HXX-FILE-NAME "ils_nils.hxx")' \
		'(HXX-FILE-PATH "$(TDIR)/ils_n_t.hxx")'
	diff $(TARGETDIRFP)/ils_n_t.cxx $(TARGETDIRFP)/ils_nils.cxx
	diff $(TARGETDIRFP)/ils_n_t.hxx $(TARGETDIRFP)/ils_nils.hxx
	@echo "-------------------------------------------------------------"
	@echo "    Rebuilding the binary using ILS                          "
	@echo "-------------------------------------------------------------"
	$(MAKE) ils_new ILS=$(TARGETDIRFP)/ils_nils
	@echo "-------------------------------------------------------------"
	@echo "    Testing the newly-built binary                           "
	@echo "-------------------------------------------------------------"
	$(TARGETDIRFP)/ils_new ils.scm \
		'(CXX-FILE-PATH "$(TDIR)/ils_t.cxx")' \
		'(HXX-FILE-NAME "ils.hxx")' \
		'(HXX-FILE-PATH "$(TDIR)/ils_t.hxx")'
	diff $(TARGETDIRFP)/ils.cxx $(TARGETDIRFP)/ils_t.cxx
	diff $(TARGETDIRFP)/ils.hxx $(TARGETDIRFP)/ils_t.hxx
	@echo "****************************************************"
	@echo "              Everything seems O.k.  "
	@echo "****************************************************"
	mv $(TARGETDIRFP)/ils_new $(TARGETDIRFP)/ils

quick_bootstrap:
	@echo "-------------------------------------------------------------"
	@echo "    Building NILS interpreter                                "
	@echo "-------------------------------------------------------------"
	$(MAKE) nils
	@echo "-------------------------------------------------------------"
	@echo "    Building an ILS binary by self-applying ILS under NILS   "
	@echo "-------------------------------------------------------------"
	$(MAKE) ils_nils
	mv $(TARGETDIRFP)/ils_nils $(TARGETDIRFP)/ils

###############################
# Library

TARGETLIBNAME = libilldebug.a

-include $(TARGETDIRFP)/defines.mk

lib: $(TARGETDIRFP)/ssympkg.o $(TARGETDIRFP)/ils_loop.o
	$(AR) crs $(TARGETDIRFP)/$(TARGETLIBNAME) $^

lib_add: $(TARGETDIRFP)/ssympkg.o $(TARGETDIRFP)/ils_loop.o
	$(AR) rs $(TARGETDIRFP)/$(TARGETLIBNAME) $^

###############################
# Common targets

../version.h:	../Version
	cd .. && $(MAKE) version.h

clean:	FORCE	
	rm -f *.o ssympkgen *.cxx *.hxx *._sc ils nils ils_nils

	

FORCE:

