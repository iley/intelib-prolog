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

BINLIBDIR = ../build

OSTYPE = $(shell uname -s)

LOCALFLAGS = -fpic -funsigned-char -g -Wall -I./$(BINLIBDIR) 

SCHEMELIBMODULES = std str sel io rdr hsh m
LIBINTELIB = -L./$(BINLIBDIR) -lintelib -lintelib_interp
LFUNHEADERS = $(patsubst %,$(BINLIBDIR)/sch_%.hpp,$(SCHEMELIBMODULES))
LIBDEFSCMFILES = $(patsubst %,$(BINLIBDIR)/%.scm,$(SCHEMELIBMODULES)) 
LIBDEPEND = $(LFUNHEADERS) $(BINLIBDIR)/libintelib.a \
			$(BINLIBDIR)/libintelib_interp.a


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


NILS = ./nils
ILS= ./ils

default:
	@echo "No default rule here."
	@echo "If you only need to get a working binary, the most"
	@echo "appropriate target for you is 'bootstrap'"

CXXFLAGS = $(LOCALFLAGS) 

###############################
# library symbols and other definitions

sympkgen:	sympkgen.cpp
	$(CXX) $(CXXFLAGS) $^ $(LIBINTELIB) -o $@

sympkg.cxx:	sympkgen
	cat $(LIBDEFSCMFILES) ilsbdef.scm | ./sympkgen > $@

sympkg.o:	sympkg.cxx
	$(CXX) $(CXXFLAGS) -c $^ -o $@

trtgen:		trtgen.c
	$(CXX) $(CXXFLAGS) trtgen.c -o trtgen

chartran._sc:	trtgen
	./trtgen > $@

ilsdef._sc:	chartran._sc ilsbdef.scm
	echo '(%%%' > $@
	cat $^ >> $@
	#cat ../build/*.scm >> $@
	cat $(LIBDEFSCMFILES) >> $@
	echo ')' >> $@

###############################
# Interpreter

ils_loop.o: ils_loop.cpp ils_loop.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

nils.o: nils.cpp ils_loop.hpp ../version.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

nils: nils.o ils_loop.o sympkg.o
	$(CXX) $(CXXFLAGS) $^  -o $@ \
	  $(LIBINTELIB) $(LIBMATH) $(LIBREADLINE) $(LIBDL) -Wl,-export-dynamic

###############################
# Compiler

###
# build with NILS

ils_nils.cxx ils_nils.hxx:	$(NILS)	ils.scm ilsdef._sc
	$(NILS) -i ils.scm \
		-x '(cmdlinedirectives "(MODULE-NAME \"ils_nils\")")'\
		-x '(ils_main (quote ("ilsdef._sc" "ils.scm")))' -Q

ils_nils.o:	ils_nils.cxx ils_nils.hxx ../version.h 
	$(CXX) $(CXXFLAGS) -c ils_nils.cxx -o $@

ils_nils:	ilsmain.cpp ils_nils.o $(LIBDEPEND)
	$(CXX) $(CXXFLAGS) -DHXXFILE=\"ils_nils.hxx\" ils_nils.o \
		ilsmain.cpp -o $@ $(LIBINTELIB)

###
# build with itself


ils.cxx ils.hxx:	ils.scm ilsdef._sc
	$(ILS) ils.scm	

ils.o:	ils.cxx ils.hxx 
	$(CXX) $(CXXFLAGS) -c ils.cxx -o $@

ils_new:	ils.o ilsmain.cpp $(LIBDEPEND)
	$(CXX) $(CXXFLAGS) ils.o ilsmain.cpp $(LIBINTELIB) -o $@



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
	./ils_nils ils.scm \
		'(MODULE-NAME "ils_nils")' \
		'(CXX-FILE-PATH "./ils_n_t.cxx")' \
		'(HXX-FILE-NAME "ils_nils.hxx")' \
		'(HXX-FILE-PATH "./ils_n_t.hxx")'
	diff ils_n_t.cxx ils_nils.cxx 
	diff ils_n_t.hxx ils_nils.hxx 
	@echo "-------------------------------------------------------------"
	@echo "    Rebuilding the binary using ILS                          "
	@echo "-------------------------------------------------------------"
	$(MAKE) ils_new ILS=./ils_nils
	@echo "-------------------------------------------------------------"
	@echo "    Testing the newly-built binary                           "
	@echo "-------------------------------------------------------------"
	./ils_new ils.scm \
		'(CXX-FILE-PATH "./ils_t.cxx")' \
		'(HXX-FILE-NAME "ils.hxx")' \
		'(HXX-FILE-PATH "./ils_t.hxx")'
	diff ils.cxx ils_t.cxx 
	diff ils.hxx ils_t.hxx 
	@echo "****************************************************"
	@echo "              Everything seems O.k.  "
	@echo "****************************************************"
	mv ils_new ils	

quick_bootstrap:
	@echo "-------------------------------------------------------------"
	@echo "    Building NILS interpreter                                "
	@echo "-------------------------------------------------------------"
	$(MAKE) nils
	@echo "-------------------------------------------------------------"
	@echo "    Building an ILS binary by self-applying ILS under NILS   "
	@echo "-------------------------------------------------------------"
	$(MAKE) ils_nils
	mv ils_nils ils	

###############################
# Library

TARGETDIR = $(CURDIR)

ifneq ($(TARGETDIR),$(filter /%,$(TARGETDIR)))
    TARGETDIRFP = $(CURDIR)/$(TARGETDIR)
else
    TARGETDIRFP = $(TARGETDIR)
endif

TARGETLIBNAME = libilldebug.a

-include $(TARGETDIRFP)/defines.mk

lib: sympkg.o ils_loop.o 
	$(AR) crs $(TARGETDIRFP)/$(TARGETLIBNAME) $^

lib_add: sympkg.o ils_loop.o 
	$(AR) rs $(TARGETDIRFP)/$(TARGETLIBNAME) $^

###############################
# Common targets

../version.h:	../Version
	cd .. && $(MAKE) version.h

clean:	FORCE	
	rm -f *.o sympkgen *.cxx *.hxx *._sc ils nils ils_nils

	

FORCE:

