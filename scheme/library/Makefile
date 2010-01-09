#   InteLib                                    http://www.intelib.org
#   The file scheme/library/Makefile
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
DIRS = $(SCHEMELIBMODULES) 

none:
	@echo "No default rule here"

-include ../../version.mk

SUBTARGETS= $(addsuffix /ALL,$(DIRS) )

all:	$(SUBTARGETS)

##  $(TARGETDIRFP)/genfun.scm
## 
## $(TARGETDIRFP)/genfun.scm: genfun.scm
## 	cp $^ $@

%/ALL:	% FORCE
	cd $< && $(MAKE) all TARGETDIRFP=$(TARGETDIRFP) \
			CXXFLAGS="$(CXXFLAGS)"


FORCE:
