SHELL = /bin/sh

SOURCE_FILES=$(wildcard *.cpp)

TARGET_FILES=$(addprefix $(TARGETDIRFP)/lfun_,$(SOURCE_FILES:.cpp=.o))

none:
	@echo "No default rule here"

COMMON_FILES = ../ilisplib.hpp ../ilisplib.mk

-include ../../../version.mk

$(TARGETDIRFP)/$(DIRNAME).lsp:	$(SOURCE_FILES) $(COMMON_FILES)
	echo "(ADD-LIB-HEADERS \"lfun_$(DIRNAME).hpp\")" > $@
	$(CXX) -D INTELIB_LISP_TRANSLATOR_INFORMATION \
		-include ../ilisplib.hpp -E -P $(SOURCE_FILES) >> $@

$(TARGETDIRFP)/lfun_$(DIRNAME).hpp:	$(SOURCE_FILES) $(COMMON_FILES)
	@echo '/* GENERATED FILE -- DO NOT EDIT */' > $@
	@echo '#if !defined(INTELIB_LFUN_$(DIRNAME)_SENTRY)' >> $@
	@echo '#define INTELIB_LFUN_$(DIRNAME)_SENTRY' >> $@
	[ -f $(DIRNAME)_hdr.inc ] && cat $(DIRNAME)_hdr.inc >> $@ || :
	@echo '#include "genlisp/lispform.hpp"' >> $@
	$(CXX) -D INTELIB_LISP_LIBRARY_HEADER_GENERATION \
		-include ../ilisplib.hpp -E -P $(SOURCE_FILES) >> $@
	@echo '#endif' >> $@

$(TARGETDIRFP)/lfun_%.o:	%.cpp $(COMMON_FILES)
	$(CXX) $(CXXFLAGS) -c -include ../ilisplib.hpp \
		-D INTELIB_LISP_LIBRARY_IMPLEMENTATION $< -o $@

all:	$(TARGET_FILES) $(TARGETDIRFP)/$(DIRNAME).lsp \
			$(TARGETDIRFP)/lfun_$(DIRNAME).hpp
