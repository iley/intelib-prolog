SHELL = /bin/sh

SOURCE_FILES=$(wildcard *.cpp)

TARGET_FILES=$(addprefix $(TARGETDIRFP)/glsp_,$(SOURCE_FILES:.cpp=.o))

none:
	@echo "No default rule here"

COMMON_FILES = ../genlslib.hpp ../genlslib.mk

-include ../../../version.mk

$(TARGETDIRFP)/glsp_$(DIRNAME).hpp:	$(SOURCE_FILES) $(COMMON_FILES)
	@echo '/* GENERATED FILE -- DO NOT EDIT */' > $@
	@echo '#if !defined(INTELIB_GLSP_$(DIRNAME)_SENTRY)' >> $@
	@echo '#define INTELIB_GLSP_$(DIRNAME)_SENTRY' >> $@
	@echo '#include "genlisp/lispform.hpp"' >> $@
	$(CXX) -D INTELIB_GENLISP_LIBRARY_HEADER_GENERATION \
		-include ../genlslib.hpp -E -P $(SOURCE_FILES) >> $@
	@echo '#endif' >> $@

$(TARGETDIRFP)/glsp_%.o:	%.cpp $(COMMON_FILES)
	$(CXX) $(CXXFLAGS) -c -include ../genlslib.hpp \
		-D INTELIB_GENLISP_LIBRARY_IMPLEMENTATION $< -o $@

all:	$(TARGET_FILES) $(TARGETDIRFP)/glsp_$(DIRNAME).hpp
