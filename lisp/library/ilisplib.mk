SHELL = /bin/sh

LFUN_PREFIX = $(TARGETDIRFP)/lfun_

CXXFILES = $(wildcard *.cpp)

OBJFILES = $(addprefix $(LFUN_PREFIX),$(CXXFILES:.cpp=.o))

MKFILES = $(CXXFILES:.cpp=.mk)

COMMON_FILES = ../ilisplib.hpp ../ilisplib.mk

-include ../../../version.mk


GEN_HPP = ../gen_hpp.sh

GEN_LSP = ../gen_lsp.sh

DEPSMK = $(TARGETDIRFP)/l$(DIRNAME)_deps.mk
-include $(DEPSMK)

none:
	@echo "No default rule here"


$(TARGETDIRFP)/$(DIRNAME).lsp:	$(CXXFILES) $(COMMON_FILES)
	CXX=$(CXX) $(GEN_LSP) -h "lfun_$(DIRNAME).hpp" -c "$(CXXFILES)" >> $@

$(LFUN_PREFIX)$(DIRNAME).hpp:	$(CXXFILES) $(COMMON_FILES)
	CXX=$(CXX) $(GEN_HPP) -s "INTELIB_LFUN_$(DIRNAME)_SENTRY" -h $(DIRNAME)_hdr.inc -c "$(CXXFILES)" > $@


$(LFUN_PREFIX)%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -include ../ilisplib.hpp \
		-D INTELIB_LISP_LIBRARY_IMPLEMENTATION $< -o $@

all: $(DEPSMK)	$(OBJFILES) $(TARGETDIRFP)/$(DIRNAME).lsp \
			$(LFUN_PREFIX)$(DIRNAME).hpp


$(DEPSMK): $(CXXFILES)
	echo > $@
	$(MAKE) $(MKFILES) TARGETDIRFP=$(TARGETDIRFP)

%.mk: %.cpp
	$(CXX) $(CXXFLAGS) -MM -include ../ilisplib.hpp \
		-D INTELIB_LISP_LIBRARY_IMPLEMENTATION \
		-MT $(LFUN_PREFIX)$(@:.mk=.o) $< >> $(DEPSMK)
	$(CXX) $(CXXFLAGS) -MM -include ../ilisplib.hpp \
		-D INTELIB_LISP_LIBRARY_IMPLEMENTATION \
		-MT $(DEPSMK) $< >> $(DEPSMK)
	$(CXX) $(CXXFLAGS) -MM -include ../ilisplib.hpp \
		-D INTELIB_LISP_TRANSLATOR_INFORMATION \
		-MT $(TARGETDIRFP)/$(DIRNAME).lsp $< >> $(DEPSMK)
	$(CXX) $(CXXFLAGS) -MM -include ../ilisplib.hpp \
		-D INTELIB_LISP_TRANSLATOR_INFORMATION \
		-MT $(DEPSMK) $< >> $(DEPSMK)
	$(CXX) $(CXXFLAGS) -MM -include ../ilisplib.hpp \
		-D INTELIB_LISP_LIBRARY_HEADER_GENERATION \
		-MT $(LFUN_PREFIX)$(DIRNAME).hpp $< >> $(DEPSMK)
	$(CXX) $(CXXFLAGS) -MM -include ../ilisplib.hpp \
		-D INTELIB_LISP_LIBRARY_HEADER_GENERATION \
		-MT $(DEPSMK) $< >> $(DEPSMK)

.PHONY = all none $(MKFILES)
