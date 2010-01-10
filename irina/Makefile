#   InteLib                                    http://www.intelib.org
#   The top level Makefile
# 
#   Copyright (c) Igor Bronshtein
#   Portions copyright (c) Denis Klychkov, 2010
# 
# 
#   This is free software, licensed under GNU GPL v.2
#   See the file COPYING for further details.
# 
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.

SHELL = /bin/sh

OPTIMIZATION = -O3
PROFILING =

CXXFLAGS = -Wall -ggdb -I.. $(OPTIMIZATION) $(PROFILING)
CXXFLAGS_GENFLAGS = -Wall -Wno-parentheses -ggdb -I.. -I../win_port $(OPTIMIZATION) $(PROFILING)

REFC=`which refc`
REFGO=`which refgo`

RSL = run.rsl start.rsl lexical.rsl errors.rsl checklexical.rsl syntax.rsl brackets.rsl types.rsl library.rsl checksyntax.rsl directivessyntax.rsl linker.rsl morelinker.rsl checklinker.rsl generate.rsl generatehxx.rsl generatecxx.rsl functionscode.rsl version.rsl verystart.rsl removesc.rsl compile.rsl
RSLPLUS = run.rsl+start.rsl+lexical.rsl+errors.rsl+checklexical.rsl+syntax.rsl+brackets.rsl+types.rsl+library.rsl+checksyntax.rsl+directivessyntax.rsl+linker.rsl+morelinker.rsl+checklinker.rsl+generate.rsl+generatehxx.rsl+generatecxx.rsl+functionscode.rsl+version.rsl+verystart.rsl+removesc.rsl+compile.rsl
FILES = start.ref lexical.ref errors.ref checklexical.ref syntax.ref brackets.ref types.ref library.ref checksyntax.ref directivessyntax.ref linker.ref morelinker.ref checklinker.ref generate.ref generatehxx.ref generatecxx.ref functionscode.ref version.ref verystart.ref removesc.ref
IRINA_VERSION = 0.3.0

TARGETDIR = ../build

ifneq ($(TARGETDIR),$(filter /%,$(TARGETDIR)))
TARGETDIRFP = $(CURDIR)/$(TARGETDIR)/intelib
else
TARGETDIRFP = $(TARGETDIR)/intelib
endif

$(TARGETDIRFP)/irina: $(TARGETDIRFP)/irina.o $(TARGETDIRFP)/run.o $(TARGETDIRFP)/def_run.o\
    $(TARGETDIRFP)/libsexpr.a $(TARGETDIRFP)/libirefal.a
	$(CXX) $(CXXFLAGS) $(TARGETDIRFP)/irina.o $(TARGETDIRFP)/run.o $(TARGETDIRFP)/win_port.o \
		-L$(TARGETDIRFP) -lsexpr -lirefal -o $@

$(TARGETDIRFP)/version.ref: version_generic.ref
	sed 's/IRINA_VERSION/'$(IRINA_VERSION)'/g' version_generic.ref > $(TARGETDIRFP)/version.ref

$(TARGETDIRFP)/compile.ref: compile_generic.ref
	sed 's/CXX/'$(CXX)'/g' compile_generic.ref > $(TARGETDIRFP)/compile.ref

$(TARGETDIRFP)/%.rsl:	%.ref
	$(REFC) $<

xx: $(TARGETDIRFP)/$(RSL) $(TARGETDIRFP)/wrap
	$(TARGETDIRFP)/wrap $(FILES)

$(TARGETDIRFP)/irina.o: irina.cxx
	$(CXX) $(CXXFLAGS_GENFLAGS) -c $< -O0 -o $@

$(TARGETDIRFP)/run.o: run.cpp
	$(CXX) $(CXXFLAGS_GENFLAGS) "-DIRINA_VERSION=\"$(IRINA_VERSION)\"" "-DCXX=\"$(CXX)\"" -c $< -o $@

$(TARGETDIRFP)/def_run.o: def_run.cpp
	$(CXX) $(CXXFLAGS_GENFLAGS) -c $< -o $@

$(TARGETDIRFP)/wrap: wrap.cpp $(TARGETDIRFP)/$(RSL)
	$(CXX) "-DREFGO=\"$(REFGO)\"" "-DRSLPLUS=\"$(RSLPLUS)\"" $(CXXFLAGS) wrap.cpp -o $@

###############################
# Build our very own library binary version

$(TARGETDIRFP):
	mkdir -p $(TARGETDIRFP)

$(TARGETDIRFP)/libsexpr.a:	$(TARGETDIRFP) FORCE
	cd ../sexpress && $(MAKE) all TARGETDIR=$(TARGETDIRFP)/.. \
		OPTIMIZATION=$(OPTIMIZATION) PROFILING=$(PROFILING)

$(TARGETDIRFP)/libirefal.a:	$(TARGETDIRFP) FORCE
	cd ../refal && $(MAKE) all TARGETDIR=$(TARGETDIRFP)/.. \
		OPTIMIZATION=$(OPTIMIZATION) PROFILING=$(PROFILING)

###############################

clean:
	rm -rf $(TARGETDIRFP)/*.rsl $(TARGETDIRFP)/*.o $(TARGETDIRFP)/core $(TARGETDIRFP)/buf \
	$(TARGETDIRFP)/irina $(TARGETDIRFP)/wrap $(TARGETDIRFP)/version.ref $(TARGETDIRFP)/compile.ref

FORCE:
	:

all: $(TARGETDIRFP)/wrap $(TARGETDIRFP)/irina

###############################
