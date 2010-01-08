SHELL = /bin/sh

GLSP_PREFIX=$(TARGETDIRFP)/glsp_

CXXFILES=$(wildcard *.cpp)

OBJFILES=$(addprefix $(GLSP_PREFIX)$(DIRNAME),$(CXXFILES:.cpp=.o))

COMMON_FILES = ../genlslib.hpp ../genlslib.mk

-include ../../../version.mk

GEN_HPP = ../gen_hpp.sh

GEN_DEPSMK = ../../../gen_deps_mk.sh


DEPSMK = $(TARGETDIRFP)/gl$(DIRNAME)_deps.mk
-include $(DEPSMK)

none: FORCE
	@echo "No default rule here"

$(GLSP_PREFIX)$(DIRNAME).hpp:	$(CXXFILES) $(COMMON_FILES)
	CXX=$(CXX) $(GEN_HPP) -s "INTELIB_GLSP_$(DIRNAME)_SENTRY" -c "$(CXXFILES)" > $@

$(GLSP_PREFIX)$(DIRNAME)%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c -include ../genlslib.hpp \
		-D INTELIB_GENLISP_LIBRARY_IMPLEMENTATION $< -o $@

all: $(DEPSMK) $(OBJFILES) $(GLSP_PREFIX)$(DIRNAME).hpp FORCE

$(DEPSMK):
	echo > $@
	$(GEN_DEPSMK) --cxx "$(CXX)" \
		--cxxflags "$(CXXFLAGS) -include ../genlslib.hpp -D INTELIB_GENLISP_LIBRARY_IMPLEMENTATION" \
		--prefix "$(GLSP_PREFIX)$(DIRNAME)" \
		--files "$(CXXFILES)" \
		--deps-mk "$@"
	$(GEN_DEPSMK) --cxx "$(CXX)" \
		--cxxflags "$(CXXFLAGS) -include ../genlslib.hpp -D INTELIB_GENLISP_LIBRARY_HEADER_GENERATION" \
		--output "$(GLSP_PREFIX)$(DIRNAME).hpp" \
		--files "$(CXXFILES)" \
		--deps-mk "$@"

FORCE:
