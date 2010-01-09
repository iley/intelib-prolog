#   InteLib                                    http://www.intelib.org
#   The file ill/Makefile
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

TARGETDIR = ../build
ifneq ($(TARGETDIR),$(filter /%,$(TARGETDIR)))
TARGETDIRFP = $(CURDIR)/$(TARGETDIR)/intelib
else
TARGETDIRFP = $(TARGETDIR)/intelib
endif
#TARGETDIRFP = bin_lib

OSTYPE = $(shell uname -s)

DEPSMK = $(TARGETDIRFP)/ill_deps.mk

GEN_DEPSMK = ../gen_deps_mk.sh



LOCALFLAGS = -fpic -funsigned-char -g -Wall -I$(TARGETDIRFP)/.. -I$(TARGETDIRFP)

LIBINTELIB = -L$(TARGETDIRFP) -lintelib -lintelib_interp
FUNHEADERS = $(patsubst %,$(TARGETDIRFP)/lfun_%.hpp,$(LISPLIBMODULES))
LIBDEFLSPFILES = $(patsubst %,$(TARGETDIRFP)/%.lsp,$(LISPLIBMODULES))
#LIBDEPEND = $(LFUNHEADERS) $(TARGETDIRFP)/libilisp.a $(TARGETDIRFP)/libsexpr.a
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


NILL = $(TARGETDIRFP)/nill
ILL = $(TARGETDIRFP)/ill

default:
	@echo "No default rule here."
	@echo "If you only need to get a working binary, the most"
	@echo "appropriate target for you is 'bootstrap'"

CXXFLAGS = $(LOCALFLAGS) 

#########################################################################


$(TARGETDIRFP)/libintelib.a $(TARGETDIRFP)/libintelib_nill.a $(LFUNHEADERS):
	cd .. && $(MAKE) library TARGETDIR=$(TARGETDIRFP)/..


###############################
# Build with nill 

$(TARGETDIRFP)/ill_nill.cxx $(TARGETDIRFP)/ill_nill.hxx:	$(NILL)	ill.lsp nill_run.lsp $(TARGETDIRFP)/illdef._ls
	cp nill_run.lsp ill.lsp $(TARGETDIRFP)
	cd $(TARGETDIRFP) && $(NILL) $(TARGETDIRFP)/nill_run.lsp

$(TARGETDIRFP)/ill_nill.o:	$(TARGETDIRFP)/ill_nill.cxx $(TARGETDIRFP)/ill_nill.hxx ../version.h
	$(CXX) $(CXXFLAGS) -c $< -o $@


ill_nill: $(TARGETDIRFP)/ill_nill
	:


$(TARGETDIRFP)/ill_nill:	illmain.cpp $(TARGETDIRFP)/ill_nill.o $(LIBDEPEND)
	@echo "-------------------------------------------------------------"
	@echo "    Building an ILL binary by self-applying ILL under NILL   "
	@echo "-------------------------------------------------------------"
	$(CXX) $(CXXFLAGS) -DHXXFILE=\"ill_nill.hxx\" $(TARGETDIRFP)/ill_nill.o \
		illmain.cpp -o $@ \
		$(LIBINTELIB)



#########################
# Build a new ILL binary with ILL itself

$(TARGETDIRFP)/ill.cxx $(TARGETDIRFP)/ill.hxx:	ill.lsp $(TARGETDIRFP)/illdef._ls
	cp ill.lsp $(TARGETDIRFP)
	cd $(TARGETDIRFP) && $(ILL) $(TARGETDIRFP)/ill.lsp

$(TARGETDIRFP)/ill.o:	$(TARGETDIRFP)/ill.cxx $(TARGETDIRFP)/ill.hxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

ill_new: $(TARGETDIRFP)/ill_new
	:


$(TARGETDIRFP)/ill_new:	$(TARGETDIRFP)/ill.o illmain.cpp $(LIBDEPEND)
	@echo "-------------------------------------------------------------"
	@echo "    Rebuilding the binary using ILL                          "
	@echo "-------------------------------------------------------------"
	$(CXX) $(CXXFLAGS) $(TARGETDIRFP)/ill.o illmain.cpp $(LIBINTELIB) -o $@


#########################
# Bootstrap 

bootstrap: FORCE
	$(MAKE) nill TARGETDIR=$(TARGETDIRFP)/..
	$(MAKE) ill_nill TARGETDIR=$(TARGETDIRFP)/..
	@echo "-------------------------------------------------------------"
	@echo "    Testing the newly-built binary                           "
	@echo "-------------------------------------------------------------"
	$(TARGETDIRFP)/ill_nill ill.lsp \
		'(MODULE-NAME "ill_nill")' \
		'(CXX-FILE-PATH "$(TDIR)/ill_n_t.cxx")' \
		'(HXX-FILE-NAME "ill_nill.hxx")' \
		'(HXX-FILE-PATH "$(TDIR)/ill_n_t.hxx")'
	diff $(TARGETDIRFP)/ill_n_t.cxx $(TARGETDIRFP)/ill_nill.cxx
	diff $(TARGETDIRFP)/ill_n_t.hxx $(TARGETDIRFP)/ill_nill.hxx
	$(MAKE) ill_new ILL=$(TARGETDIRFP)/ill_nill
	@echo "-------------------------------------------------------------"
	@echo "    Testing the newly-built binary                           "
	@echo "-------------------------------------------------------------"
	$(TARGETDIRFP)/ill_new ill.lsp \
		'(CXX-FILE-PATH "$(TDIR)/ill_t.cxx")' \
		'(HXX-FILE-NAME "ill.hxx")' \
		'(HXX-FILE-PATH "$(TDIR)/ill_t.hxx")'
	diff $(TARGETDIRFP)/ill.cxx $(TARGETDIRFP)/ill_t.cxx
	diff $(TARGETDIRFP)/ill.hxx $(TARGETDIRFP)/ill_t.hxx
	@echo "****************************************************"
	@echo "              Everything seems O.k.  "
	@echo "****************************************************"
	mv $(TARGETDIRFP)/ill_new $(TARGETDIRFP)/ill

quick_bootstrap: FORCE
	$(MAKE) nill
	$(MAKE) ill_nill
	mv $(TARGETDIRFP)/ill_nill $(TARGETDIRFP)/ill




###############################
# Interpreter


TARGETLIBNAME = libilldebug.a

-include $(TARGETDIRFP)/defines.mk

lib: $(TARGETDIRFP)/sympkg.o $(TARGETDIRFP)/ill_loop.o $(TARGETDIRFP)/illtrace.o
	$(AR) crs $(TARGETDIRFP)/$(TARGETLIBNAME) $^

lib_add: $(TARGETDIRFP)/sympkg.o $(TARGETDIRFP)/ill_loop.o $(TARGETDIRFP)/illtrace.o
	$(AR) rs $(TARGETDIRFP)/$(TARGETLIBNAME) $^



$(TARGETDIRFP)/sympkgen: sympkgen.cpp $(LIBDEPEND)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBINTELIB)

$(TARGETDIRFP)/sympkg.cxx: $(TARGETDIRFP)/sympkgen $(TARGETDIRFP)/illdef._ls
	$(TARGETDIRFP)/sympkgen < $(TARGETDIRFP)/illdef._ls > $@

$(TARGETDIRFP)/sympkg.o: $(TARGETDIRFP)/sympkg.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

$(TARGETDIRFP)/illtrace.o: illtrace.cpp illtrace.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

$(TARGETDIRFP)/ill_loop.o: ill_loop.cpp ill_loop.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

$(TARGETDIRFP)/nill.o: nill.cpp ill_loop.hpp ../version.h
	$(CXX) $(CXXFLAGS) $(INCDL) -c nill.cpp -o $@


nill: $(TARGETDIRFP)/nill
	:

$(TARGETDIRFP)/nill: $(TARGETDIRFP)/nill.o $(TARGETDIRFP)/sympkg.o $(TARGETDIRFP)/ill_loop.o $(TARGETDIRFP)/illtrace.o
	@echo "-------------------------------------------------------------"
	@echo "    Building NILL interpreter                                "
	@echo "-------------------------------------------------------------"
	$(CXX) $(CXXFLAGS) $^  -o $@ \
	  $(LIBINTELIB) $(LIBMATH) $(LIBREADLINE) $(LIBDL) -Wl,$(DYNAMIC)


$(DEPSMK): Makefile
	echo > $@
	$(GEN_DEPSMK) --cxx "$(CXX)" \
		--cxxflags "$(CXXFLAGS)" \
		--output "$(TARGETDIRFP)/sympkgen" \
		--files "sympkgen.cpp" \
		--deps-mk "$@"
	$(GEN_DEPSMK) --cxx "$(CXX)" \
		--cxxflags "$(CXXFLAGS)" \
		--prefix "$(TARGETDIRFP)/" \
		--files "sympkg.cxx illtrace.cpp ill_loop.cpp" \
		--deps-mk "$@"

###############################
# Common targets

../version.h:	../Version
	cd .. && $(MAKE) version.h


$(TARGETDIRFP)/trtgen:		trtgen.c
	$(CXX) $(CXXFLAGS) trtgen.c -o $@

$(TARGETDIRFP)/chartran._ls:	$(TARGETDIRFP)/trtgen
	$(TARGETDIRFP)/trtgen > $@

#lib._ls:	$(LIBDEFLSPFILES)
$(TARGETDIRFP)/lib._ls:	$(LIBDEPEND)
	cat $(LIBDEFLSPFILES) >  $@

$(TARGETDIRFP)/illdef._ls:	$(TARGETDIRFP)/chartran._ls $(TARGETDIRFP)/lib._ls illbdef.lsp
	echo "; InteLib Lisp default definitions, generated with" > $@
	echo ";       make illdef._ls" >> $@
	echo "(%%% " >> $@
	cat $(TARGETDIRFP)/chartran._ls illbdef.lsp $(TARGETDIRFP)/lib._ls >> $@
	echo ") ; end of definitions " >> $@


clean: FORCE
	rm -f trtgen sympkgen *.[ch]xx *.cpl *.o *._ls ill_new ill_nill
	rm -f $(TARGETDIRFP)/*.{o,lsp,a,hpp}
	rm -f ill nill

FORCE:
