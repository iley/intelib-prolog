#   InteLib                                    http://www.intelib.org
#   The file tests/refal/Makefile
# 
#   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
# 
# 
#   This is free software, licensed under GNU GPL v.2
#   See the file COPYING for further details.
# 
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.




ifdef TARGETDIR
-include bin_$(TARGETDIR)/defines.mk
endif

OPTIMIZATION = -O0

CXXFLAGS = -Wall -g -I.. -I../.. -Ibin_$(TARGETDIR)
CXXFLAGS_GENFLAGS = -Wall -Wno-parentheses -g -I.. -I../.. -Ibin_$(TARGETDIR)

TESTS = t_rfexpr t_markup t_vars t_match t_eval t_time
TESTS_NO_TIME = t_rfexpr t_markup t_vars t_match t_eval

TARGETDIRS = default 

ifdef TARGETDIR
BINTESTS = $(patsubst %,bin_$(TARGETDIR)/%,$(TESTS))
-include bin_$(TARGETDIR)/defines.mk
endif

ALLSOURCES = $(wildcard *.[ch]pp)
SOURCEFILES = $(TESTS:=.cpp) ../tests.cpp

RSL = run.rsl start.rsl lexical.rsl errors.rsl checklexical.rsl syntax.rsl brackets.rsl types.rsl library.rsl checksyntax.rsl directivessyntax.rsl linker.rsl morelinker.rsl checklinker.rsl generate.rsl generatehxx.rsl generatecxx.rsl functionscode.rsl version.rsl verystart.rsl removesc.rsl compile.rsl

rund: $(RSL)
	$(MAKE) run TARGETDIRS=default

%.rsl:
	[ -h $@ ] || ln -s ../../irina/$@ $@

briefd:
	$(MAKE) brief TARGETDIRS=default


run:
	./links.sh
	cd ../../irina && $(MAKE) wrap && $(MAKE) irina
	for D in $(TARGETDIRS) ; do \
		$(MAKE) run_dir TARGETDIR=$$D "TESTS=$(TESTS)" ; \
	done

notime:
	$(MAKE) runnotime TARGETDIRS=default

runnotime:
	cd ../../irina && $(MAKE) irina
	for D in $(TARGETDIRS) ; do \
		$(MAKE) run_dir TARGETDIR=$$D "TESTS=$(TESTS_NO_TIME)" ; \
	done

run_dir: $(BINTESTS)
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

LIB_OBJECTS = bin_$(TARGETDIR)/libirefal.a bin_$(TARGETDIR)/libsexpr.a 

bin_$(TARGETDIR):
	mkdir -p $@

bin_$(TARGETDIR)/%:	%.cpp ../tests.o $(LIB_OBJECTS)
	$(CXX) $(CXXFLAGS_GENFLAGS) $^ -o $@ $(CONDDEFINES) 

bin_$(TARGETDIR)/libsexpr.a:	bin_$(TARGETDIR) FORCE
	cd ../../sexpress && $(MAKE) all \
		TARGETDIR=../tests/refal/bin_$(TARGETDIR) \
		OPTIMIZATION=$(OPTIMIZATION)

bin_$(TARGETDIR)/libirefal.a:	bin_$(TARGETDIR) FORCE
	cd ../../refal && $(MAKE) all \
		TARGETDIR=../tests/refal/bin_$(TARGETDIR) \
		OPTIMIZATION=$(OPTIMIZATION) 

bin_$(TARGETDIR)/defines.mk: bin_$(TARGETDIR) $(TARGETDIR)_defines.mk
	cp $(TARGETDIR)_defines.mk bin_$(TARGETDIR)/defines.mk
	
endif


clean:
	rm -f *.o tests.log time.log test*.*[xy][xy] *.rsl
	./unlinks.sh
	for D in $(TARGETDIRS) ; do rm -rf bin_$$D ; done

FORCE:
	:
