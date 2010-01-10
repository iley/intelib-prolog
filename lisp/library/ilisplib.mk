#   InteLib                                    http://www.intelib.org
#   The file lisp/library/ilisplib.mk
# 
#   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
# 
# 
#   This is free software, licensed under GNU GPL v.2
#   See the file COPYING for further details.
# 
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.


SHELL = /bin/sh

LFUN_PREFIX = $(TARGETDIRFP)/lfun_

CXXFILES = $(wildcard *.cpp)

OBJFILES = $(addprefix $(LFUN_PREFIX)$(DIRNAME),$(CXXFILES:.cpp=.o))

MKFILES = $(CXXFILES:.cpp=.mk)

COMMON_FILES = ../ilisplib.hpp ../ilisplib.mk

-include ../../../version.mk


GEN_HPP = ../gen_hpp.sh

GEN_LSP = ../gen_lsp.sh

GEN_DEPSMK = ../../../gen_deps_mk.sh


DEPSMK = $(TARGETDIRFP)/l$(DIRNAME)_deps.mk
-include $(DEPSMK)

none: FORCE
	@echo "No default rule here"


$(TARGETDIRFP)/$(DIRNAME).lsp:	$(CXXFILES) $(COMMON_FILES)
	CXX=$(CXX) $(GEN_LSP) -h "lfun_$(DIRNAME).hpp" -c "$(CXXFILES)" >> $@

$(LFUN_PREFIX)$(DIRNAME).hpp:	$(CXXFILES) $(COMMON_FILES)
	CXX=$(CXX) $(GEN_HPP) -s "INTELIB_LFUN_$(DIRNAME)_SENTRY" -h $(DIRNAME)_hdr.inc -c "$(CXXFILES)" > $@


$(LFUN_PREFIX)$(DIRNAME)%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -include ../ilisplib.hpp \
		-D INTELIB_LISP_LIBRARY_IMPLEMENTATION $< -o $@

all: $(DEPSMK)	$(OBJFILES) $(TARGETDIRFP)/$(DIRNAME).lsp FORCE \
			$(LFUN_PREFIX)$(DIRNAME).hpp

$(DEPSMK):
	echo > $@
	$(GEN_DEPSMK) --cxx "$(CXX)" \
		--cxxflags "$(CXXFLAGS) -include ../ilisplib.hpp -D INTELIB_LISP_LIBRARY_IMPLEMENTATION" \
		--prefix "$(LFUN_PREFIX)$(DIRNAME)" \
		--files "$(CXXFILES)" \
		--deps-mk "$@"
	$(GEN_DEPSMK) --cxx $(CXX) \
		--cxxflags "$(CXXFLAGS) -include ../ilisplib.hpp -D INTELIB_LISP_LIBRARY_HEADER_GENERATION" \
		--output "$(LFUN_PREFIX)$(DIRNAME).hpp"
		--files "$(CXXFILES)" \
		--deps-mk "$@"
	$(GEN_DEPSMK) --cxx $(CXX) \
		--cxxflags "$(CXXFLAGS) -include ../ilisplib.hpp -D INTELIB_LISP_TRANSLATOR_INFORMATION" \
		--output $(TARGETDIRFP)/$(DIRNAME).lsp \
		--files "$(CXXFILES)" \
		--deps-mk "$@"

FORCE:
