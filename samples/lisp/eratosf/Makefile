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
