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
