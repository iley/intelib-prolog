#   InteLib                                    http://www.intelib.org
#   The file samples/lisp/isomorph/Makefile
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

CXXFLAGS = -Wall -g $(INTELIBFLAGS)

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

prog:	isomorph.o main.cpp
	$(CXX) $(CXXFLAGS) $^ -lintelib -o $@ 

run:	prog
	./prog

lrun:
	$(CLISP) go.lsp

clean:
	rm -f *.o prog
