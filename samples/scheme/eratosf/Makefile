#   InteLib                                    http://www.intelib.org
#   The file samples/scheme/eratosf/Makefile
# 
#   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
# 
# 
#   This is free software, licensed under GNU GPL v.2
#   See the file COPYING for further details.
# 
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.




#
# Use 'INTELIB=../../..' if InteLib is compiled but not installed
#

ifneq ($(INTELIB),)
ILS = $(INTELIB)/ils/ils
NILS = $(INTELIB)/ils/nils
LIBDIR = $(INTELIB)/build
INTELIBFLAGS = -I$(LIBDIR) -L$(LIBDIR)
else
ILS = ils
NILS = nils
INTELIBFLAGS = 
endif

CXXFLAGS = -Wall -g

default:
	@echo "No default rule"
	@echo 'Try "make run" or "make nrun"'


eratosf.cxx eratosf.hxx: eratosf.scm
	$(ILS) $<

eratosf:	main.cpp eratosf.o 
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS) $^ -lintelib -o $@

		
eratosf.o:	eratosf.cxx eratosf.hxx 
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS) -c $< -o $@

run:	eratosf
	./eratosf

nrun:
	$(NILS) -i eratosf.scm -x '(display (eratosfen 100)) (newline)' -Q

clean:
	rm -f *.o eratosf *.[ch]xx
