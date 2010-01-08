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

# Try 'make bootstrap' as a good quick start. 
# See docs/build.txt file for the details about building the library
# and bootstrapping the translator.


# The directory in which the library will be built
TARGETDIR = ./build

ifneq ($(TARGETDIR),$(filter /%,$(TARGETDIR)))
TARGETDIRFP = $(CURDIR)/$(TARGETDIR)/intelib
else
TARGETDIRFP = $(TARGETDIR)/intelib
endif

# comment this out to get .o files removed after build
KEEP_OBJECTS = yes

USE_READLINE = yes

OPTIMIZATION = -O2

VERSION_SUFFIX = $(word 1, $(shell head -1 Version))

#########################################
# Various install tree descriptions

# Anyway Windows does not support symlinks as well as *nix
ifeq ($(OSTYPE),MinGW-win)
INSTALLMODE = native
endif

ifeq ($(INSTALLMODE),native) 

    # 'native' install (choose ONE prefix)
PREFIX = /usr/local
#PREFIX=/usr
SYMLINK_PREFIX = 
INCLUDEDIR = $(PREFIX)/include/intelib
INCLUDEDIR_SYMLINK = no
#DOCDIR=$(PREFIX)/share/docs   # not used in this version

else

# 'separate directory' install
SYSPREFIX = /usr/local
PREFIX = $(SYSPREFIX)/intelib-$(VERSION_SUFFIX)
ifneq ($(findstring -$(VERSION_SUFFIX),$(PREFIX)),)
  SYMLINK_PREFIX = $(subst -$(VERSION_SUFFIX),,$(PREFIX))
else
  SYMLINK_PREFIX = $(PREFIX)
endif
INCLUDEDIR = $(PREFIX)/include
INCLUDEDIR_SYMLINK = yes
#DOCDIR=$(PREFIX)/share/docs   # not used in this version

endif

BINDIR = $(PREFIX)/bin
LIBDIR = $(PREFIX)/lib


INSTALL = install
INSTALL_DIR = $(INSTALL) -d
INSTALL_HEADERS = $(INSTALL) -m 0644
INSTALL_BIN = $(INSTALL) -m 0755
INSTALL_LIB = $(INSTALL) -m 0644
INSTALL_DATA = $(INSTALL) -m 0644

LDCONFIG = /sbin/ldconfig

#########################################

default:	bootstrap FORCE
	:

bootstrap: FORCE
	$(MAKE) library USE_READLINE=$(USE_READLINE)
	cd ils && $(MAKE) bootstrap USE_READLINE=$(USE_READLINE)
	cd ill && $(MAKE) bootstrap USE_READLINE=$(USE_READLINE)
ifneq ($(OSTYPE),MinGW-win)
# This feature still not supported in windows build.
	[ -d irina ] && cd irina && $(MAKE)
endif

libintelib.a: win_port FORCE
	cd sexpress && $(MAKE) all TARGETDIR=$(TARGETDIRFP)/.. \
				OPTIMIZATION=$(OPTIMIZATION) \
				TARGETLIBNAME=$@
	cd tools && $(MAKE) all_add TARGETDIR=$(TARGETDIRFP)/.. \
				OPTIMIZATION=$(OPTIMIZATION) \
				TARGETLIBNAME=$@
	cd genlisp && $(MAKE) all_add TARGETDIR=$(TARGETDIRFP)/.. \
				OPTIMIZATION=$(OPTIMIZATION) \
				TARGETLIBNAME=$@
	cd scheme && $(MAKE) all_add TARGETDIR=$(TARGETDIRFP)/.. \
				OPTIMIZATION=$(OPTIMIZATION) \
				TARGETLIBNAME=$@
	cd lisp && $(MAKE) all_add TARGETDIR=$(TARGETDIRFP)/.. \
				OPTIMIZATION=$(OPTIMIZATION) \
				TARGETLIBNAME=$@
	[ -d refal ] && cd refal && $(MAKE) all_add TARGETDIR=$(TARGETDIRFP)/.. \
				OPTIMIZATION=$(OPTIMIZATION) \
				USE_READLINE=$(USE_READLINE) \
				TARGETLIBNAME=$@

libintelib_interp.a: win_port FORCE
	cd interact && $(MAKE) all TARGETDIR=$(TARGETDIRFP)/.. \
				OPTIMIZATION=$(OPTIMIZATION) \
				USE_READLINE=$(USE_READLINE) \
				TARGETLIBNAME=$@
	cd ils && $(MAKE) lib_add TARGETDIR=$(TARGETDIRFP)/.. \
				OPTIMIZATION=$(OPTIMIZATION) \
				USE_READLINE=$(USE_READLINE) \
				TARGETLIBNAME=$@
	cd ill && $(MAKE) lib_add TARGETDIR=$(TARGETDIRFP)/.. \
				OPTIMIZATION=$(OPTIMIZATION) \
				USE_READLINE=$(USE_READLINE) \
				TARGETLIBNAME=$@

library: $(TARGETDIRFP) FORCE
ifeq ($(OSTYPE),MinGW-win)
# Windows version - without symlinks. Just copy needed files.
	cp -R $(CURDIR)/sexpress $(TARGETDIRFP)
	cp -R $(CURDIR)/tools $(TARGETDIRFP)
	cp -R $(CURDIR)/genlisp $(TARGETDIRFP)
	cp -R $(CURDIR)/scheme $(TARGETDIRFP)
	cp -R $(CURDIR)/lisp $(TARGETDIRFP)
	cp -R $(CURDIR)/interact $(TARGETDIRFP)
else
	ln -sf $(CURDIR)/sexpress $(TARGETDIRFP)
	ln -sf $(CURDIR)/tools $(TARGETDIRFP)
	ln -sf $(CURDIR)/genlisp $(TARGETDIRFP)
	ln -sf $(CURDIR)/scheme $(TARGETDIRFP)
	ln -sf $(CURDIR)/lisp $(TARGETDIRFP)
	ln -sf $(CURDIR)/interact $(TARGETDIRFP)
endif
#	ln -sf $(TARGETDIRFP) $(TARGETDIRFP)/intelib
	$(MAKE) libintelib.a
	$(MAKE) libintelib_interp.a
ifeq ($(KEEP_OBJECTS),)
	rm $(TARGETDIRFP)/*.o
endif

$(TARGETDIRFP):	
	mkdir -p $(TARGETDIRFP)

win_port: $(TARGETDIRFP) FORCE
	cd win_port && $(MAKE) TARGETDIR=$(TARGETDIRFP)/..
	
version.h:	Version
	echo '#define INTELIB_VERSION "'`head -1 Version`'"' > $@
	echo '#define INTELIB_VERSIONID '`tail -1 Version` >> $@

doxydocs: FORCE
	doxygen docs/doxygen/intelib.conf


install: FORCE
		# as the interact/ directory presently doesn't hold files,
		# it is not to be mentioned here
	for D in sexpress tools genlisp lisp scheme ill ils refal ; do \
		$(INSTALL_DIR) $(DESTDIR)$(INCLUDEDIR)/$$D ; \
		$(INSTALL_HEADERS) $$D/*.hpp  $(DESTDIR)$(INCLUDEDIR)/$$D ; \
	done
	$(INSTALL_HEADERS) $(TARGETDIRFP)/*.hpp $(DESTDIR)$(INCLUDEDIR)
	$(INSTALL_DIR) $(DESTDIR)$(LIBDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL_LIB) $(TARGETDIRFP)/*.a $(DESTDIR)$(LIBDIR)
	$(INSTALL_BIN) $(TARGETDIRFP)/ill $(TARGETDIRFP)/nill $(DESTDIR)$(BINDIR)
	$(INSTALL_BIN) $(TARGETDIRFP)/ils $(TARGETDIRFP)/nils $(DESTDIR)$(BINDIR)
	$(INSTALL_DATA) $(TARGETDIRFP)/illdef._ls $(DESTDIR)$(BINDIR)
	$(INSTALL_DATA) $(TARGETDIRFP)/ilsdef._sc $(DESTDIR)$(BINDIR)
ifndef DESTDIR
ifeq ($(INCLUDEDIR_SYMLINK),yes)
	cd $(DESTDIR)$(INCLUDEDIR) && ( [ -e intelib ] || ln -sfn . intelib )
endif
ifneq ($(SYMLINK_PREFIX),)
ifneq ($(SYMLINK_PREFIX),$(PREFIX))
	ln -sfn $(PREFIX) $(SYMLINK_PREFIX)
endif
	$(INSTALL_DIR) $(SYSPREFIX)/bin $(SYSPREFIX)/lib
	ln -sf $(SYMLINK_PREFIX)/bin/ill $(SYSPREFIX)/bin
	ln -sf $(SYMLINK_PREFIX)/bin/nill $(SYSPREFIX)/bin
	ln -sf $(SYMLINK_PREFIX)/bin/ils $(SYSPREFIX)/bin
	ln -sf $(SYMLINK_PREFIX)/bin/nils $(SYSPREFIX)/bin
	ln -sf $(SYMLINK_PREFIX)/bin/illdef._ls $(SYSPREFIX)/bin
	ln -sf $(SYMLINK_PREFIX)/bin/ilsdef._sc $(SYSPREFIX)/bin
	ln -sf $(SYMLINK_PREFIX)/lib/libintelib* $(SYSPREFIX)/lib
	$(INSTALL_DIR) $(SYSPREFIX)/include
	[ -e $(SYSPREFIX)/include/intelib ] || \
		ln -sf $(SYMLINK_PREFIX)/include $(SYSPREFIX)/include/intelib
endif
## commented out: we don't produce any shared libraries
#  ifneq ($(LDCONFIG),)
#  	$(LDCONFIG) $(LIBDIR)
#  endif
endif


clean: FORCE
	cd samples && $(MAKE) clean TARGETDIR=$(TARGETDIRFP)/.. || :
	cd tests && $(MAKE) clean TARGETDIR=$(TARGETDIRFP)/.. || :
	rm -rf $(TARGETDIR) docs/doxygen/html
	rm -rf docs/doxygen/man

FORCE:
