SHELL = /bin/sh

SCH_PREFIX=$(TARGETDIRFP)/sch_

CXXFILES=$(wildcard *.cpp)

OBJFILES=$(addprefix $(SCH_PREFIX)$(DIRNAME),$(CXXFILES:.cpp=.o))

MKFILES = $(CXXFILES:.cpp=.mk)

COMMON_FILES = ../schemlib.hpp ../schemlib.mk

-include ../../../version.mk


GEN_HPP = ../gen_hpp.sh

GEN_SCM = ../gen_scm.sh

GEN_DEPSMK = ../../../gen_deps_mk.sh


DEPSMK = $(TARGETDIRFP)/sch$(DIRNAME)_deps.mk
-include $(DEPSMK)

none: FORCE
	@echo "No default rule here"


$(TARGETDIRFP)/$(DIRNAME).scm:	$(CXXFILES) $(COMMON_FILES)
	CXX=$(CXX) $(GEN_SCM) -h "sch_$(DIRNAME).hpp" -c "$(CXXFILES)" > $@

$(SCH_PREFIX)$(DIRNAME).hpp:	$(CXXFILES) $(COMMON_FILES)
	CXX=$(CXX) $(GEN_HPP) -s "INTELIB_SCHEMLIB_$(DIRNAME)_SENTRY" -h $(DIRNAME)_hdr.inc -c "$(CXXFILES)" > $@


$(SCH_PREFIX)$(DIRNAME)%.o:	%.cpp $(COMMON_FILES)
	$(CXX) $(CXXFLAGS) -c -include ../schemlib.hpp \
		-D INTELIB_SCHEME_LIBRARY_IMPLEMENTATION $< -o $@

all: $(DEPSMK) $(OBJFILES) $(TARGETDIRFP)/$(DIRNAME).scm FORCE \
			$(SCH_PREFIX)$(DIRNAME).hpp

$(DEPSMK):
	$(GEN_DEPSMK) --cxx "$(CXX)" \
		--cxxflags "$(CXXFLAGS) -include ../schemlib.hpp -D INTELIB_SCHEME_LIBRARY_IMPLEMENTATION" \
		--prefix "$(SCH_PREFIX)$(DIRNAME)" \
		--files "$(CXXFILES)" \
		--deps-mk "$@"
	$(GEN_DEPSMK) --cxx "$(CXX)" \
		--cxxflags "$(CXXFLAGS) -include ../schemlib.hpp -D INTELIB_SCHEME_LIBRARY_HEADER_GENERATION" \
		--prefix "$(SCH_PREFIX)" \
		--files "$(CXXFILES)" \
		--suffix "hpp" \
		--deps-mk "$@"
	$(GEN_DEPSMK) --cxx "$(CXX)" \
		--cxxflags "$(CXXFLAGS) -include ../schemlib.hpp -D INTELIB_SCHEME_TRANSLATOR_INFORMATION" \
		--prefix "$(SCH_PREFIX)" \
		--files "$(CXXFILES)" \
		--suffix "scm" \
		--deps-mk "$@"

FORCE:
