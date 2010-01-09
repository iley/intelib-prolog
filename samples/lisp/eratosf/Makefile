#   InteLib                                    http://www.intelib.org
#   The file samples/lisp/eratosf/Makefile
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

CLISP = clisp

CXXFLAGS = -Wall -g

default:
	@echo "No default rule"
	@echo 'Try "make run" or "make lrun"'


ifneq ($(ILLDEBUG),yes)

prog.cxx prog.hxx: prog.lsp 
	$(ILL) prog.lsp

prog:	prog.cpp progxx.o 
	$(CXX) -Wall -ggdb $(INTELIBFLAGS) \
		progxx.o prog.cpp -lintelib -o $@

else

prog.cxx prog.hxx prog_debugger.cxx: prog.lsp 
	$(ILL) prog.lsp '(DEBUGGER-FILE)' 

prog_debugger.o:	prog_debugger.cxx prog.hxx 
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS) -c prog_debugger.cxx -o $@

prog:	prog.cpp progxx.o prog_debugger.o 
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS) \
		progxx.o prog_debugger.o prog.cpp \
		-lintelib_interp -lintelib -lreadline -o $@

endif
		
progxx.o:	prog.cxx prog.hxx 
	$(CXX) $(CXXFLAGS) $(INTELIBFLAGS) -c prog.cxx -o $@

run:	prog
	./prog


lrun:
	$(CLISP) go.lsp

clean:
	rm -f *.o prog *.[ch]xx
