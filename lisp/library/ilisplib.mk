SHELL = /bin/sh

LFUN_PREFIX = $(TARGETDIRFP)/lfun_

CXXFILES = $(wildcard *.cpp)

OBJFILES = $(addprefix $(LFUN_PREFIX),$(CXXFILES:.cpp=.o))

MKFILES = $(CXXFILES:.cpp=.mk)

COMMON_FILES = ../ilisplib.hpp ../ilisplib.mk

-include ../../../version.mk


GEN_HPP = ../gen_hpp.sh


none:
	@echo "No default rule here"


$(TARGETDIRFP)/$(DIRNAME).lsp:	$(CXXFILES) $(COMMON_FILES)
	echo "(ADD-LIB-HEADERS \"lfun_$(DIRNAME).hpp\")" > $@
	$(CXX) -D INTELIB_LISP_TRANSLATOR_INFORMATION \
		-include ../ilisplib.hpp -E -P $(CXXFILES) >> $@

$(LFUN_PREFIX)$(DIRNAME).hpp:	$(CXXFILES) $(COMMON_FILES)
	$(GEN_HPP) -s "INTELIB_LFUN_$(DIRNAME)_SENTRY" -h $(DIRNAME)_hdr.inc > $@


$(LFUN_PREFIX)%.o:
	$(CXX) $(CXXFLAGS) -c -include ../ilisplib.hpp \
		-D INTELIB_LISP_LIBRARY_IMPLEMENTATION $< -o $@

all:	$(OBJFILES) $(TARGETDIRFP)/$(DIRNAME).lsp \
			$(TARGETDIRFP)/lfun_$(DIRNAME).hpp

-include deps.mk

deps.mk:
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
