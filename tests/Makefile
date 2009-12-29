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




TESTDIRS= sexpress lisp scheme refal prolog
OPTIMIZATION = -O0
INCDIRS = -I. -I.. -I../..
CXXFLAGS = $(OPTIMIZATION) -ggdb -Wall $(INCDIRS) $(CONDDEFINES)


rund run:
	for D in $(TESTDIRS) ; do \
		([ -d $$D ] && cd $$D && $(MAKE) $(MAKECMDGOALS)) || true; \
	done

brief:
	$(MAKE) run | grep '^@@@ \*'

briefd:
	$(MAKE) rund | grep '^@@@ \*'

log:
	$(MAKE) run > tests.log 2>&1 

tests.o:	tests.cpp tests.hpp
	$(CXX) $(CXXFLAGS) tests.cpp -c

clean:
	rm -f tests.log tests.o
	for D in $(TESTDIRS) ; do \
		[ -d $$D ] && (cd $$D && $(MAKE) clean) || true; \
	done
