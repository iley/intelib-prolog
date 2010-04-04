#   InteLib                                    http://www.intelib.org
#   The file tests/scheme/Makefile
#
#   Copyright (c) Vasiliy Kulikov, 2010
#
#
#   This is free software, licensed under GNU GPL v.2
#   See the file COPYING for further details.
#
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.



TESTS = t_cont t_funstd t_schxvr

LIB_OBJECTS = bin_$(TARGETDIR)/intelib/libischeme.a \
	bin_$(TARGETDIR)/intelib/libintelibgenlisp.a \
	bin_$(TARGETDIR)/intelib/libintelibtools.a \
	bin_$(TARGETDIR)/intelib/libsexpr.a

bin_$(TARGETDIR)/t_funm:	t_funm.cpp ../tests.o $(LIB_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lm $(CONDDEFINES)

include ../common_tests.mk

