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





ifdef TARGETDIR
-include bin_$(TARGETDIR)/defines.mk
endif

OPTIMIZATION = -O0

CXXFLAGS= -Wall -g -I.. -I../.. -Ibin_$(TARGETDIR)

TESTS = t_lisp t_lsymb t_eval t_form t_leaks t_thread \
	t_funstd t_funstr t_funrdr t_funhsh t_funm \
	t_lextv t_bndset

TARGETDIRS = default 
#TARGETDIRS = default longnumbers effective scheme

ifdef TARGETDIR
BINTESTS = $(patsubst %,bin_$(TARGETDIR)/%,$(TESTS))
-include bin_$(TARGETDIR)/defines.mk
endif

ALLSOURCES = $(wildcard *.[ch]pp)
SOURCEFILES = $(TESTS:=.cpp) ../tests.cpp


rund:
	$(MAKE) run TARGETDIRS=default

briefd:
	$(MAKE) brief TARGETDIRS=default


run:
	for D in $(TARGETDIRS) ; do \
		$(MAKE) run_dir TARGETDIR=$$D "TESTS=$(TESTS)" ; \
	done


run_dir: lib_rebuild $(BINTESTS)
	@echo "@@@ * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo "@@@ * Processing directory bin_$(TARGETDIR)"
	@echo "@@@ * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	for T in $(BINTESTS) ; do \
		$$T  ; \
	done

brief:
	$(MAKE) run | grep '^@@@ \*'

log:
	$(MAKE) run > tests.log 2>&1 

../tests.o:    ../tests.cpp ../tests.hpp
	cd .. && $(MAKE) tests.o

ifdef TARGETDIR

LIB_OBJECTS = bin_$(TARGETDIR)/libilisp.a \
	bin_$(TARGETDIR)/libintelibgenlisp.a \
	bin_$(TARGETDIR)/libintelibtools.a \
	bin_$(TARGETDIR)/libsexpr.a 


bin_$(TARGETDIR):
	mkdir -p $@

bin_$(TARGETDIR)/%:		%.cpp ../tests.o $(LIB_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(CONDDEFINES) 

bin_$(TARGETDIR)/t_funm:	t_funm.cpp ../tests.o $(LIB_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lm $(CONDDEFINES) 

bin_$(TARGETDIR)/libsexpr.a:	bin_$(TARGETDIR)
	cd ../../sexpress && $(MAKE) all \
		TARGETDIR=../tests/lisp/bin_$(TARGETDIR) \
		OPTIMIZATION=$(OPTIMIZATION)

bin_$(TARGETDIR)/libintelibtools.a:	bin_$(TARGETDIR)
	cd ../../tools && $(MAKE) all \
		TARGETDIR=../tests/lisp/bin_$(TARGETDIR) \
		OPTIMIZATION=$(OPTIMIZATION)

bin_$(TARGETDIR)/libintelibgenlisp.a:	bin_$(TARGETDIR)
	cd ../../genlisp && $(MAKE) all \
		TARGETDIR=../tests/lisp/bin_$(TARGETDIR) \
		OPTIMIZATION=$(OPTIMIZATION)

bin_$(TARGETDIR)/libilisp.a:	bin_$(TARGETDIR) 
	cd ../../lisp && $(MAKE) all \
		TARGETDIR=../tests/lisp/bin_$(TARGETDIR) \
		OPTIMIZATION=$(OPTIMIZATION)

lib_rebuild:
	rm -f $(LIB_OBJECTS)
	$(MAKE) $(LIB_OBJECTS)

bin_$(TARGETDIR)/defines.mk: bin_$(TARGETDIR) $(TARGETDIR)_defines.mk
	cp $(TARGETDIR)_defines.mk bin_$(TARGETDIR)/defines.mk
	
endif


clean:
	rm -f *.o tests.log
	for D in $(TARGETDIRS) ; do rm -rf bin_$$D ; done

FORCE:
	:
