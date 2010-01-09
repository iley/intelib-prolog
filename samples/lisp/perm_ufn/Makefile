#   InteLib                                    http://www.intelib.org
#   The file samples/lisp/perm_ufn/Makefile
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

prog:	main.cpp ppr.o perm.o 
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS)\
		ppr.o perm.o main.cpp -lintelib -o $@

else

ppr.cxx ppr.hxx ppr_debugger.cxx: ppr.lsp 
	$(ILL) ppr.lsp '(DEBUGGER-FILE)' 

ppr_debugger.o:	ppr_debugger.cxx ppr.hxx 
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS) -c ppr_debugger.cxx -o $@

prog:	main.cpp ppr.o ppr_debugger.o perm.o  
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS) \
		ppr.o perm.o ppr_debugger.o main.cpp \
		-lintelib_interp -lintelib -lreadline -o $@

endif
		
ppr.o:	ppr.cxx ppr.hxx 
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS) -c $< -o $@

perm.o:	perm.cpp perm.hpp
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS) -c $< -o $@


nill_lib.o:	nill_lib.cpp perm.hpp
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS) -c $< -o $@

nill_perm.so:	nill_lib.o perm.o
	$(CXX) -shared -fpic $^ -o $@

run:	prog
	./prog

 
clean:
	rm -f *.o prog *.[ch]xx nill_perm.so
