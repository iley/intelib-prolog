#  InteLib                                    http://www.intelib.org
#  The file win_port/Makefile
#
#  Copyright (c) Denis Klychkov, 2010
#
#
#  This is free software, licensed under GNU LGPL v.2.1
#  See the file COPYING for further details.
#
#  THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#  Please see the file WARRANTY for the detailed explanation.

SHELL = /bin/sh

TARGETDIR = ../build

ifneq ($(TARGETDIR),$(filter /%,$(TARGETDIR)))
TARGETDIRFP = $(CURDIR)/$(TARGETDIR)/intelib
else
TARGETDIRFP = $(TARGETDIR)/intelib
endif

CC = gcc

LOCALFLAGS = -fpic -funsigned-char -g -Wall

CXXFLAGS = $(LOCALFLAGS) 

ifeq ($(OSTYPE), MinGW-win)
LIBDL = $(TARGETDIRFP)/libdl.a
else
LIBDL = 
endif

default: $(LIBDL) $(TARGETDIRFP)/win_port.o

$(TARGETDIRFP)/win_port.o:	win_port.hpp win_port.cpp
	$(CXX) $(CXXFLAGS) -c win_port.cpp -o $(TARGETDIRFP)/win_port.o

$(TARGETDIRFP)/libdl.a: $(TARGETDIRFP)/dlfcn.o
	ar cru $(TARGETDIRFP)/libdl.a $(TARGETDIRFP)/dlfcn.o
	ranlib $(TARGETDIRFP)/libdl.a

$(TARGETDIRFP)/dlfcn.o:
	$(CC) -o $(TARGETDIRFP)/dlfcn.o -c dlfcn.c -Wall -O3 -fomit-frame-pointer

clean:
	rm -f $(TARGETDIRFP)/win_port.o $(TARGETDIRFP)/dlfcn.o $(LIBDL)
