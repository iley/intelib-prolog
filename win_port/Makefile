# +-------------------------------------------------------------------------+
# |               I__n__t__e__L__i__b           0.6.21 development          |
# | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
# |                                                                         |
# | This is free software.  See GNU GENERAL PUBLIC LICENSE v.2 for details. |
# | GNU GPL v2  is  found  in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
# |     Please see also docs/readme.txt and visit http://www.intelib.org    |
# |                                                                         |
# | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
# +-------------------------------------------------------------------------+

SHELL = /bin/sh

TARGETDIR = ../build

ifneq ($(TARGETDIR),$(filter /%,$(TARGETDIR)))
TARGETDIRFP = $(CURDIR)/$(TARGETDIR)/intelib
else
TARGETDIRFP = $(TARGETDIR)/intelib
endif

DLFCN=dlfcn-win32-r11

LOCALFLAGS = -fpic -funsigned-char -g -Wall

CXXFLAGS = $(LOCALFLAGS) 

default: dlfcn win_port.o

win_port.o:	win_port.hpp win_port.cpp
	$(CXX) $(CXXFLAGS) -c win_port.cpp -o $(TARGETDIRFP)/win_port.o

dlfcn:
	cd $(DLFCN) && configure && $(MAKE)
	cp $(DLFCN)/dlfcn.h .
	cp $(DLFCN)/libdl.a .
	
clean:
	rm -f $(TARGETDIRFP)/win_port.o libdl.a dlfcn.h
	cd $(DLFCN) && $(MAKE) clean
