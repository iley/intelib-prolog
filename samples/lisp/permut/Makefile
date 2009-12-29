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
ILL = $(INTELIB)/ill/ill
LIBDIR = $(INTELIB)/build
INTELIBFLAGS = -I$(LIBDIR) -L$(LIBDIR)
else
ILL = ill
INTELIBFLAGS = 
endif

CXXFLAGS = -Wall -ggdb -fpic

default:
	@echo "No default rule"
	@echo 'Try "make run"'


ifneq ($(ILLDEBUG),yes)

ppr.cxx ppr.hxx: ppr.lsp 
	$(ILL) ppr.lsp

prog:	main.cpp ppr.o perm.o symbs.o 
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS)\
		ppr.o perm.o symbs.o main.cpp -lintelib -o $@

else

ppr.cxx ppr.hxx ppr_debugger.cxx: ppr.lsp 
	$(ILL) ppr.lsp '(DEBUGGER-FILE)' 

ppr_debugger.o:	ppr_debugger.cxx ppr.hxx 
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS) -c ppr_debugger.cxx -o $@

prog:	main.cpp ppr.o ppr_debugger.o perm.o symbs.o 
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS) \
		ppr.o perm.o symbs.o ppr_debugger.o main.cpp \
		-lintelib_interp -lintelib -lreadline -o $@

endif
		
ppr.o:	ppr.cxx ppr.hxx 
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS) -c $< -o $@

perm.o:	perm.cpp perm.hpp
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS) -c $< -o $@

symbs.o:	symbs.cpp symbs.hpp perm.hpp
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS) -c $< -o $@

nill_lib.o:	nill_lib.cpp perm.hpp
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS) -c $< -o $@

nill_perm.so:	nill_lib.o perm.o
	$(CXX) -shared -fpic $^ -o $@

run:	prog
	./prog

 
clean:
	rm -f *.o prog *.[ch]xx nill_perm.so
