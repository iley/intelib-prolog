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
CLISP = clisp
CXXFLAGS = -Wall -ggdb -I$(INTELIB) -I$(LIBDIR) -L$(LIBDIR) 
else
ILL = ill
CXXFLAGS = -Wall -ggdb 
endif


default:
	@echo "No default rule"
	@echo 'Try "make run"'


##############
# Primary translation method

prog.cxx prog.hxx: prog.lsp 
	$(ILL) prog.lsp

prog:	prog.cpp progxx.o 
	$(CXX)  $(CXXFLAGS) progxx.o prog.cpp -lintelib -o $@

		
progxx.o:	prog.cxx prog.hxx
	$(CXX) $(CXXFLAGS) -c prog.cxx -o $@

run:	prog
	./prog


ifneq ($(LIBDIR),$(INTELIB)/build)

$(LIBDIR)/libintelib.a $(LIBDIR)/lfun_std.hpp: $(LIBDIR)/defines.mk
	cd $(INTELIB) && make library TARGETDIR=$(CURDIR)/$(LIBDIR)\
			 ILLDEBUG=$(ILLDEBUG) KEEP_OBJECTS=yes

$(LIBDIR):
	mkdir $(LIBDIR)

$(LIBDIR)/defines.mk: $(LIBDIR) defines.mk
	cp defines.mk $(LIBDIR)

$(ILL):
	cd $(INTELIB) && make bootstrap

endif


clean:
	rm -rf $(LIBDIR)
	rm -f *.o prog *.[ch]xx
