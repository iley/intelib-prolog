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
#BINLIBDIR = bin_lib

OSTYPE = $(shell uname -s)


LOCALFLAGS = -fpic -funsigned-char -g -Wall -I./$(BINLIBDIR) 

#LIBINTELIB = -L./$(BINLIBDIR) -lilisp -lsexpr
LIBINTELIB = -L./$(BINLIBDIR) -lintelib -lintelib_interp
LFUNHEADERS = $(patsubst %,$(BINLIBDIR)/lfun_%.hpp,$(LISPLIBMODULES))
LIBDEFLSPFILES = $(patsubst %,$(BINLIBDIR)/%.lsp,$(LISPLIBMODULES))
#LIBDEPEND = $(LFUNHEADERS) $(BINLIBDIR)/libilisp.a $(BINLIBDIR)/libsexpr.a
LIBDEPEND = $(LFUNHEADERS) $(BINLIBDIR)/libintelib.a \
			$(BINLIBDIR)/libintelib_interp.a


ifeq ($(OSTYPE),Linux)
    LIBDL = -ldl
else 
    LIBDL = 
endif

ifeq ($(LISPMATH),yes)
    LISPLIBMODULES = std sel str io rdr hsh m
    LIBMATH = -lm
else 
    LISPLIBMODULES = std sel str io rdr hsh
    LIBMATH = 
endif

ifeq ($(USE_READLINE),yes)
    #LIBREADLINE = -lreadline /lib/libtinfo.so.5
    LIBREADLINE = -lreadline
    USEREADLINEFLAG = -DINTELIB_USE_READLINE=1
else 
    LIBREADLINE = 
    USEREADLINEFLAG = -DINTELIB_USE_READLINE=0
endif


NILL = ./nill
ILL = ./ill

default:
	@echo "No default rule here."
	@echo "If you only need to get a working binary, the most"
	@echo "appropriate target for you is 'bootstrap'"

CXXFLAGS = $(LOCALFLAGS) 

#########################################################################


$(BINLIBDIR)/libintelib.a $(BINLIBDIR)/libintelib_nill.a $(LFUNHEADERS):	
	cd .. && $(MAKE) library TARGETDIR=ill/$(BINLIBDIR)


###############################
# Build with nill 

ill_nill.cxx ill_nill.hxx:	$(NILL)	ill.lsp nill_run.lsp illdef._ls
	$(NILL) nill_run.lsp

ill_nill.o:	ill_nill.cxx ill_nill.hxx ../version.h 
	$(CXX) $(CXXFLAGS) -c ill_nill.cxx -o $@

ill_nill:	illmain.cpp ill_nill.o $(LIBDEPEND)
	$(CXX) $(CXXFLAGS) -DHXXFILE=\"ill_nill.hxx\" ill_nill.o \
		illmain.cpp -o $@ $(LIBINTELIB)



#########################
# Build a new ILL binary with ILL itself

ill.cxx ill.hxx:	ill.lsp illdef._ls
	$(ILL) ill.lsp	

ill.o:	ill.cxx ill.hxx 
	$(CXX) $(CXXFLAGS) -c ill.cxx -o $@

ill_new:	ill.o illmain.cpp $(LIBDEPEND)
	$(CXX) $(CXXFLAGS) ill.o illmain.cpp $(LIBINTELIB) -o $@


#########################
# Bootstrap 

bootstrap:
	@echo "-------------------------------------------------------------"
	@echo "    Building NILL interpreter                                "
	@echo "-------------------------------------------------------------"
	$(MAKE) nill
	@echo "-------------------------------------------------------------"
	@echo "    Building an ILL binary by self-applying ILL under NILL   "
	@echo "-------------------------------------------------------------"
	$(MAKE) ill_nill
	@echo "-------------------------------------------------------------"
	@echo "    Testing the newly-built binary                           "
	@echo "-------------------------------------------------------------"
	./ill_nill ill.lsp \
		'(MODULE-NAME "ill_nill")' \
		'(CXX-FILE-PATH "./ill_n_t.cxx")' \
		'(HXX-FILE-NAME "ill_nill.hxx")' \
		'(HXX-FILE-PATH "./ill_n_t.hxx")'
	diff ill_n_t.cxx ill_nill.cxx 
	diff ill_n_t.hxx ill_nill.hxx 
	@echo "-------------------------------------------------------------"
	@echo "    Rebuilding the binary using ILL                          "
	@echo "-------------------------------------------------------------"
	$(MAKE) ill_new ILL=./ill_nill
	@echo "-------------------------------------------------------------"
	@echo "    Testing the newly-built binary                           "
	@echo "-------------------------------------------------------------"
	./ill_new ill.lsp \
		'(CXX-FILE-PATH "./ill_t.cxx")' \
		'(HXX-FILE-NAME "ill.hxx")' \
		'(HXX-FILE-PATH "./ill_t.hxx")'
	diff ill.cxx ill_t.cxx 
	diff ill.hxx ill_t.hxx 
	@echo "****************************************************"
	@echo "              Everything seems O.k.  "
	@echo "****************************************************"
	mv ill_new ill	

quick_bootstrap:
	@echo "-------------------------------------------------------------"
	@echo "    Building NILL interpreter                                "
	@echo "-------------------------------------------------------------"
	$(MAKE) nill
	@echo "-------------------------------------------------------------"
	@echo "    Building an ILL binary by self-applying ILL under NILL   "
	@echo "-------------------------------------------------------------"
	$(MAKE) ill_nill
	mv ill_nill ill	




###############################
# Interpreter

TARGETDIR = $(CURDIR)

ifneq ($(TARGETDIR),$(filter /%,$(TARGETDIR)))
    TARGETDIRFP = $(CURDIR)/$(TARGETDIR)
else
    TARGETDIRFP = $(TARGETDIR)
endif

TARGETLIBNAME = libilldebug.a

-include $(TARGETDIRFP)/defines.mk

lib: sympkg.o ill_loop.o illtrace.o
	$(AR) crs $(TARGETDIRFP)/$(TARGETLIBNAME) $^

lib_add: sympkg.o ill_loop.o illtrace.o
	$(AR) rs $(TARGETDIRFP)/$(TARGETLIBNAME) $^



sympkgen: sympkgen.cpp $(LIBDEPEND)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBINTELIB)

sympkg.cxx: sympkgen illdef._ls
	./sympkgen < illdef._ls > $@

sympkg.o: sympkg.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

illtrace.o: illtrace.cpp illtrace.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

ill_loop.o: ill_loop.cpp ill_loop.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

nill.o: nill.cpp ill_loop.hpp ../version.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

nill: nill.o sympkg.o ill_loop.o illtrace.o
	$(CXX) $(CXXFLAGS) $^  -o $@ \
	  $(LIBINTELIB) $(LIBMATH) $(LIBREADLINE) $(LIBDL) -Wl,-export-dynamic

###############################
# Common targets

../version.h:	../Version
	cd .. && $(MAKE) version.h


trtgen:		trtgen.c
	$(CXX) $(CXXFLAGS) trtgen.c -o trtgen

chartran._ls:	trtgen
	./trtgen > $@

#lib._ls:	$(LIBDEFLSPFILES)
lib._ls:	$(LIBDEPEND)
	cat $(LIBDEFLSPFILES) >  $@

illdef._ls:	chartran._ls lib._ls illbdef.lsp
	echo "; InteLib Lisp default definitions, generated with" > $@
	echo ";       make illdef._ls" >> $@
	echo "(%%% " >> $@
	cat chartran._ls illbdef.lsp lib._ls >> $@
	echo ") ; end of definitions " >> $@


clean:
	rm -f trtgen sympkgen *.[ch]xx *.cpl *.o *._ls ill_new ill_nill
	rm -f $(BINLIBDIR)/*.{o,lsp,a,hpp}
	rm -f ill nill
	
.PHONY: clean bootstrap quick_bootstrap
