SHELL = /bin/sh

LFUN_PREFIX = $(TARGETDIRFP)/lfun_

CXXFILES = $(wildcard *.cpp)

OBJFILES = $(addprefix $(LFUN_PREFIX),$(CXXFILES:.cpp=.o))

MKFILES = $(CXXFILES:.cpp=.mk)

none:
	@echo "No default rule here"

COMMON_FILES = ../ilisplib.hpp ../ilisplib.mk

-include ../../../version.mk

$(TARGETDIRFP)/$(DIRNAME).lsp:	$(CXXFILES) $(COMMON_FILES)
	echo "(ADD-LIB-HEADERS \"lfun_$(DIRNAME).hpp\")" > $@
	$(CXX) -D INTELIB_LISP_TRANSLATOR_INFORMATION \
		-include ../ilisplib.hpp -E -P $(CXXFILES) >> $@

$(LFUN_PREFIX)$(DIRNAME).hpp:	$(CXXFILES) $(COMMON_FILES)
	@echo '/* GENERATED FILE -- DO NOT EDIT */' > $@
	@echo '#if !defined(INTELIB_LFUN_$(DIRNAME)_SENTRY)' >> $@
	@echo '#define INTELIB_LFUN_$(DIRNAME)_SENTRY' >> $@
	[ -f $(DIRNAME)_hdr.inc ] && cat $(DIRNAME)_hdr.inc >> $@ || :
	@echo '#include "genlisp/lispform.hpp"' >> $@
	$(CXX) -D INTELIB_LISP_LIBRARY_HEADER_GENERATION \
		-include ../ilisplib.hpp -E -P $(CXXFILES) >> $@
	@echo '#endif' >> $@

$(LFUN_PREFIX)%.o:	%.cpp $(COMMON_FILES)
	$(CXX) $(CXXFLAGS) -c -include ../ilisplib.hpp \
		-D INTELIB_LISP_LIBRARY_IMPLEMENTATION $< -o $@

all:	$(OBJFILES) $(TARGETDIRFP)/$(DIRNAME).lsp \
			$(TARGETDIRFP)/lfun_$(DIRNAME).hpp

-include deps.mk

deps.mk: $(CXXFILES)
	rm -f deps.mk
	touch deps.mk
	$(MAKE) -f ../ilisplib.mk $(MKFILES)

%.mk: %.cpp
	$(CXX) $(CXXFLAGS) -MM -include ../ilisplib.hpp \
		-D INTELIB_LISP_LIBRARY_IMPLEMENTATION \
		-MT $(LFUN_PREFIX)$(@:.mk=.o) $< >> deps.mk
	$(CXX) $(CXXFLAGS) -MM -include ../ilisplib.hpp \
		-D INTELIB_LISP_LIBRARY_IMPLEMENTATION \
		-MT deps.mk $< >> deps.mk
