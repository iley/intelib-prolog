#   InteLib                                    http://www.intelib.org
#   The file tests/Makefile
# 
#   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
# 
# 
#   This is free software, licensed under GNU GPL v.2
#   See the file COPYING for further details.
# 
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.




TESTDIRS= sexpress lisp scheme prolog
#TESTDIRS= sexpress lisp scheme refal prolog
OPTIMIZATION = -O0
INCDIRS = -I. -I.. -I../..
CXXFLAGS = $(OPTIMIZATION) -ggdb -Wall $(INCDIRS) $(CONDDEFINES)


rund run:
	for D in $(TESTDIRS) ; do \
		([ -d $$D -a -d ../$$D ] && cd $$D && $(MAKE) $(MAKECMDGOALS))\
			|| true; \
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
