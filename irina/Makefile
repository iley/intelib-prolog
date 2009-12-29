# INTELIB_CONTRIBUTED
# Written by Igor Bronstein (<igor.bronstein_AT_intelib.org>)

OPTIMIZATION = -O3
PROFILING =

CXXFLAGS = -Wall -ggdb -I.. $(OPTIMIZATION) $(PROFILING)
CXXFLAGS_GENFLAGS = -Wall -Wno-parentheses -ggdb -I.. $(OPTIMIZATION) $(PROFILING)

REFC=`which refc`
REFGO=`which refgo`

RSL = run.rsl start.rsl lexical.rsl errors.rsl checklexical.rsl syntax.rsl brackets.rsl types.rsl library.rsl checksyntax.rsl directivessyntax.rsl linker.rsl morelinker.rsl checklinker.rsl generate.rsl generatehxx.rsl generatecxx.rsl functionscode.rsl version.rsl verystart.rsl removesc.rsl compile.rsl
RSLPLUS = run.rsl+start.rsl+lexical.rsl+errors.rsl+checklexical.rsl+syntax.rsl+brackets.rsl+types.rsl+library.rsl+checksyntax.rsl+directivessyntax.rsl+linker.rsl+morelinker.rsl+checklinker.rsl+generate.rsl+generatehxx.rsl+generatecxx.rsl+functionscode.rsl+version.rsl+verystart.rsl+removesc.rsl+compile.rsl
FILES = start.ref lexical.ref errors.ref checklexical.ref syntax.ref brackets.ref types.ref library.ref checksyntax.ref directivessyntax.ref linker.ref morelinker.ref checklinker.ref generate.ref generatehxx.ref generatecxx.ref functionscode.ref version.ref verystart.ref removesc.ref
IRINA_VERSION = 0.3.0

irina: irina.o run.o def_run.o bin_lib/libsexpr.a bin_lib/libirefal.a
	$(CXX) $(CXXFLAGS) irina.o run.o -Lbin_lib -lsexpr\
				-lirefal -o $@

version.ref: version_generic.ref
	sed 's/IRINA_VERSION/'$(IRINA_VERSION)'/g' version_generic.ref > version.ref

compile.ref: compile_generic.ref
	sed 's/CXX/'$(CXX)'/g' compile_generic.ref > compile.ref

%.rsl:	%.ref
	$(REFC) $<

xx: $(RSL) wrap
	./wrap $(FILES)

irina.o: irina.cxx
	$(CXX) $(CXXFLAGS_GENFLAGS) -c $< -O0

run.o: run.cpp
	$(CXX) $(CXXFLAGS_GENFLAGS) "-DIRINA_VERSION=\"$(IRINA_VERSION)\"" "-DCXX=\"$(CXX)\"" -c $<

def_run.o: def_run.cpp
	$(CXX) $(CXXFLAGS_GENFLAGS) -c $<

wrap: wrap.cpp $(RSL)
	$(CXX) "-DREFGO=\"$(REFGO)\"" "-DRSLPLUS=\"$(RSLPLUS)\"" $(CXXFLAGS) wrap.cpp -o $@

###############################
# Build our very own library binary version

bin_lib:
	mkdir bin_lib

bin_lib/libsexpr.a:	bin_lib	FORCE
	cd ../sexpress && $(MAKE) all TARGETDIR=../irina/bin_lib \
		OPTIMIZATION=$(OPTIMIZATION) PROFILING=$(PROFILING)

bin_lib/libirefal.a:	bin_lib FORCE
	cd ../refal && $(MAKE) all TARGETDIR=../irina/bin_lib \
		OPTIMIZATION=$(OPTIMIZATION) PROFILING=$(PROFILING)

###############################

clean:
	rm -rf *.rsl *.o core buf irina wrap bin_lib version.ref compile.ref

FORCE:
	:

all: wrap irina

###############################
