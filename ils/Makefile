# +-------------------------------------------------------------------------+
# |               I__n__t__e__L__i__b           0.6.21 development          |
# | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
# |                                                                         |
# | This is free software.  See GNU GENERAL PUBLIC LICENSE v.2 for details. |
# | GNU GPL v2  is  found  in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
# |     Please see also docs/readme.txt and visit http://www.intelib.org    |
# |                                                                         |
# | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
# +-------------------------------------------------------------------------+




SHELL = /bin/sh

# change this to "no" if you don't want math functions (sin, cos, etc...)
# in the Lisp interpreter
LISPMATH=yes

# change this to "no" if you don't have GNU readline development files
USE_READLINE=yes

# The directory in which the library will be built
TARGETDIR = ../build

ifneq ($(TARGETDIR),$(filter /%,$(TARGETDIR)))
TARGETDIRFP = $(CURDIR)/$(TARGETDIR)
else
TARGETDIRFP = $(TARGETDIR)
endif

OSTYPE = $(shell uname -s)

LOCALFLAGS = -fpic -funsigned-char -g -Wall -I$(TARGETDIRFP)

SCHEMELIBMODULES = std str sel io rdr hsh m
LIBINTELIB = -L$(TARGETDIRFP) -lintelib -lintelib_interp
LFUNHEADERS = $(patsubst %,$(TARGETDIRFP)/sch_%.hpp,$(SCHEMELIBMODULES))
LIBDEFSCMFILES = $(patsubst %,$(TARGETDIRFP)/%.scm,$(SCHEMELIBMODULES))
LIBDEPEND = $(LFUNHEADERS) $(TARGETDIRFP)/libintelib.a \
			$(TARGETDIRFP)/libintelib_interp.a


ifeq ($(OSTYPE),Linux)
    LIBDL = -ldl
else 
    LIBDL = 
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

$(TARGETDIRFP)/chartran._sc:	$(TARGETDIRFP)/strtgen
	$(TARGETDIRFP)/strtgen > $@

$(TARGETDIRFP)/ilsdef._sc:	$(TARGETDIRFP)/chartran._sc ilsbdef.scm
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
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

nils: $(TARGETDIRFP)/nils
	:

$(TARGETDIRFP)/nils: $(TARGETDIRFP)/nils.o $(TARGETDIRFP)/ils_loop.o $(TARGETDIRFP)/ssympkg.o
	$(CXX) $(CXXFLAGS) $^  -o $@ \
	  $(LIBINTELIB) $(LIBMATH) $(LIBREADLINE) $(LIBDL) -Wl,-export-dynamic

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
		'(CXX-FILE-PATH "$(TARGETDIRFP)/ils_n_t.cxx")' \
		'(HXX-FILE-NAME "ils_nils.hxx")' \
		'(HXX-FILE-PATH "$(TARGETDIRFP)/ils_n_t.hxx")'
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
		'(CXX-FILE-PATH "$(TARGETDIRFP)/ils_t.cxx")' \
		'(HXX-FILE-NAME "ils.hxx")' \
		'(HXX-FILE-PATH "$(TARGETDIRFP)/ils_t.hxx")'
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
