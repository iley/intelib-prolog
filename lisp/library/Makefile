#   InteLib                                    http://www.intelib.org
#   The file lisp/library/Makefile
# 
#   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
# 
# 
#   This is free software, licensed under GNU LGPL v.2.1
#   See the file COPYING for further details.
# 
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.




SHELL = /bin/sh

CXX=g++
TARGETDIRFP = $(CURDIR)/..
DIRS = $(LISPLIBMODULES) 

none:
	@echo "No default rule here"

-include ../../version.mk

SUBTARGETS= $(addsuffix /ALL,$(DIRS) )

all:	$(SUBTARGETS) 

## $(TARGETDIRFP)/genfun.lsp

## $(TARGETDIRFP)/genfun.lsp: genfun.lsp
## 	cp $^ $@

%/ALL:	% FORCE
	cd $< && $(MAKE) all TARGETDIRFP=$(TARGETDIRFP) \
			CXXFLAGS="$(CXXFLAGS)"


FORCE:
