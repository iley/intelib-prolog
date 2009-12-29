SHELL = /bin/sh

SOURCE_FILES=$(wildcard *.cpp)

TARGET_FILES=$(addprefix $(TARGETDIRFP)/sch_,$(SOURCE_FILES:.cpp=.o))

none:
	@echo "No default rule here"

COMMON_FILES = ../schemlib.hpp ../schemlib.mk

-include ../../../version.mk

$(TARGETDIRFP)/$(DIRNAME).scm:	$(SOURCE_FILES) $(COMMON_FILES)
	echo "(ADD-LIB-HEADERS \"sch_$(DIRNAME).hpp\")" > $@
	$(CXX) -D INTELIB_SCHEME_TRANSLATOR_INFORMATION \
		-include ../schemlib.hpp -E -P $(SOURCE_FILES) >> $@

$(TARGETDIRFP)/sch_$(DIRNAME).hpp:	$(SOURCE_FILES) $(COMMON_FILES)
	@echo '/* GENERATED FILE -- DO NOT EDIT */' > $@
	@echo '#if !defined(INTELIB_SCHEMLIB_$(DIRNAME)_SENTRY)' >> $@
	@echo '#define INTELIB_SCHEMLIB_$(DIRNAME)_SENTRY' >> $@
	[ -f $(DIRNAME)_hdr.inc ] && cat $(DIRNAME)_hdr.inc >> $@ || :
	$(CXX) -D INTELIB_SCHEME_LIBRARY_HEADER_GENERATION \
		-include ../schemlib.hpp -E -P $(SOURCE_FILES) >> $@
	@echo '#endif' >> $@

$(TARGETDIRFP)/sch_%.o:	%.cpp $(COMMON_FILES)
	$(CXX) $(CXXFLAGS) -c -include ../schemlib.hpp \
		-D INTELIB_SCHEME_LIBRARY_IMPLEMENTATION $< -o $@

all:	$(TARGET_FILES) $(TARGETDIRFP)/$(DIRNAME).scm \
			$(TARGETDIRFP)/sch_$(DIRNAME).hpp
