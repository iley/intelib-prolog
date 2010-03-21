#   InteLib                                    http://www.intelib.org
#   The file tests/sexpress/Makefile
# 
#   Copyright (c) Andrey Vikt. Stolyarov, 2000-2010
# 
# 
#   This is free software, licensed under GNU GPL v.2
#   See the file COPYING for further details.
# 
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.





ifdef TARGETDIR
-include bin_$(TARGETDIR)/intelib/defines.mk
endif

OPTIMIZATION = -O0

CXXFLAGS= -Wall -g -I.. -I../..

TESTS = t_sizes t_refcount t_sexpr t_ix t_leaks t_vector t_backln\
	t_wrap t_stream t_lexer t_reader t_queue t_hashtb 

TARGETDIRS = default 
#TARGETDIRS = default longnumbers effective scheme

ifdef TARGETDIR
BINTESTS = $(patsubst %,bin_$(TARGETDIR)/%,$(TESTS))
-include bin_$(TARGETDIR)/intelib/defines.mk
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


run_dir: $(BINTESTS)
	@echo "@@@ * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo "@@@ * Processing directory bin_$(TARGETDIR)"
	@echo "@@@ * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@for T in $(BINTESTS) ; do \
		$$T || echo "@@@ * $$T FAILED (AS A WHOLE)" '!!!' ; \
	done

brief:
	$(MAKE) run | grep '^@@@ \*'

log:
	$(MAKE) run > tests.log 2>&1 

../tests.o:    ../tests.cpp ../tests.hpp
	cd .. && $(MAKE) tests.o

ifdef TARGETDIR

LIB_OBJECTS = bin_$(TARGETDIR)/intelib/libintelibtools.a \
	bin_$(TARGETDIR)/intelib/libsexpr.a

bin_$(TARGETDIR):
	mkdir -p $@
	mkdir -p $@/intelib

bin_$(TARGETDIR)/%:	%.cpp ../tests.o $(LIB_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(CONDDEFINES) 

bin_$(TARGETDIR)/intelib/libsexpr.a:	bin_$(TARGETDIR) FORCE
	cd ../../sexpress && $(MAKE) all \
		TARGETDIR=../tests/sexpress/bin_$(TARGETDIR) \
		OPTIMIZATION=$(OPTIMIZATION)


bin_$(TARGETDIR)/intelib/libintelibtools.a:	bin_$(TARGETDIR) FORCE
	cd ../../tools && $(MAKE) all \
		TARGETDIR=../tests/sexpress/bin_$(TARGETDIR) \
		OPTIMIZATION=$(OPTIMIZATION)

bin_$(TARGETDIR)/intelib/defines.mk: bin_$(TARGETDIR) $(TARGETDIR)_defines.mk
	cp $(TARGETDIR)_defines.mk bin_$(TARGETDIR)/intelib/defines.mk
	
endif


clean:
	rm -f *.o tests.log core
	for D in $(TARGETDIRS) ; do rm -rf bin_$$D ; done

FORCE:
	:
