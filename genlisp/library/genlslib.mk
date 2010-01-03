SHELL = /bin/sh

GLSP_PREFIX=$(TARGETDIRFP)/glsp_

CXXFILES=$(wildcard *.cpp)

OBJFILES=$(addprefix $(GLSP_PREFIX),$(CXXFILES:.cpp=.o))

MKFILES = $(CXXFILES:.cpp=.mk)

COMMON_FILES = ../genlslib.hpp ../genlslib.mk

-include ../../../version.mk


GEN_HPP = ../gen_hpp.sh



DEPSMK = $(TARGETDIRFP)/gl$(DIRNAME)_deps.mk
-include $(DEPSMK)

none:
	@echo "No default rule here"

$(GLSP_PREFIX)$(DIRNAME).hpp:	$(CXXFILES) $(COMMON_FILES)
	CXX=$(CXX) $(GEN_HPP) -s "INTELIB_GLSP_$(DIRNAME)_SENTRY" -c "$(CXXFILES)" > $@

$(GLSP_PREFIX)%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c -include ../genlslib.hpp \
		-D INTELIB_GENLISP_LIBRARY_IMPLEMENTATION $< -o $@

all: $(DEPSMK) $(OBJFILES) $(GLSP_PREFIX)$(DIRNAME).hpp


$(DEPSMK):
	echo > $@
	$(MAKE) $(MKFILES) TARGETDIRFP=$(TARGETDIRFP)

%.mk: %.cpp
	$(CXX) $(CXXFLAGS) -MM -include ../genlslib.hpp \
		-D INTELIB_GENLISP_LIBRARY_IMPLEMENTATION \
		-MT $(GLSP_PREFIX)$(@:.mk=.o) $< >> $(DEPSMK)
	$(CXX) $(CXXFLAGS) -MM -include ../genlslib.hpp \
		-D INTELIB_GENLISP_LIBRARY_IMPLEMENTATION \
		-MT $(DEPSMK) $< >> $(DEPSMK)
	$(CXX) $(CXXFLAGS) -MM -include ../genlslib.hpp \
		-D INTELIB_GENLISP_LIBRARY_HEADER_GENERATION \
		-MT $(DEPSMK) $< >> $(DEPSMK)
	$(CXX) $(CXXFLAGS) -MM -include ../genlslib.hpp \
		-D INTELIB_GENLISP_LIBRARY_HEADER_GENERATION \
		-MT $(GLSP_PREFIX)$(DIRNAME).hpp  $< >> $(DEPSMK)

.PHONY: all none $(MKFILES)
