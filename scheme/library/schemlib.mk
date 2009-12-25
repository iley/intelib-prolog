SHELL = /bin/sh

SCH_PREFIX=$(TARGETDIRFP)/sch_

CXXFILES=$(wildcard *.cpp)

OBJFILES=$(addprefix $(SCH_PREFIX),$(CXXFILES:.cpp=.o))

MKFILES = $(CXXFILES:.cpp=.mk)

COMMON_FILES = ../schemlib.hpp ../schemlib.mk

-include ../../../version.mk


GEN_HPP = ../gen_hpp.sh

GEN_SCM = ../gen_scm.sh

DEPSMK = $(TARGETDIRFP)/sch$(DIRNAME)_deps.mk
-include $(DEPSMK)

none:
	@echo "No default rule here"


$(TARGETDIRFP)/$(DIRNAME).scm:	$(CXXFILES) $(COMMON_FILES)
	CXX=$(CXX) $(GEN_SCM) -h "sch_$(DIRNAME).hpp" -c "$(CXXFILES)" > $@

$(SCH_PREFIX)$(DIRNAME).hpp:	$(CXXFILES) $(COMMON_FILES)
	CXX=$(CXX) $(GEN_HPP) -s "INTELIB_SCHEMLIB_$(DIRNAME)_SENTRY" -h $(DIRNAME)_hdr.inc -c "$(CXXFILES)" > $@


$(SCH_PREFIX)%.o:	%.cpp $(COMMON_FILES)
	$(CXX) $(CXXFLAGS) -c -include ../schemlib.hpp \
		-D INTELIB_SCHEME_LIBRARY_IMPLEMENTATION $< -o $@

all: $(DEPSMK) $(OBJFILES) $(TARGETDIRFP)/$(DIRNAME).scm \
			$(TARGETDIRFP)/sch_$(DIRNAME).hpp


$(DEPSMK): $(MKFILES)

%.mk: %.cpp
	$(CXX) $(CXXFLAGS) -MM -include ../schemlib.hpp \
		-D INTELIB_SCHEME_LIBRARY_IMPLEMENTATION \
		-MT $(LFUN_PREFIX)$(@:.mk=.o) $< >> $(DEPSMK)
	$(CXX) $(CXXFLAGS) -MM -include ../schemlib.hpp \
		-D INTELIB_SCHEME_LIBRARY_IMPLEMENTATION \
		-MT $(DEPSMK) $< >> $(DEPSMK)
