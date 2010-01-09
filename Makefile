#   InteLib                                    http://www.intelib.org
#   The file Makefile
# 
#   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
# 
# 
#   This is free software, licensed under GNU GPL v.2
#   See the file COPYING for further details.
# 
#   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
#   Please see the file WARRANTY for the detailed explanation.




SHELL=/bin/sh

# Try 'make bootstrap' as a good quick start. 
# See docs/build.txt file for the details about building the library
# and bootstrapping the translator.


# The directory in which the library will be built
TARGETDIR = ./build

# comment this out to get .o files removed after build
KEEP_OBJECTS=yes

USE_READLINE=yes

OPTIMIZATION=-O2

VERSION_SUFFIX=$(word 1, $(shell head -1 Version))

######
# Various install tree descriptions

ifeq ($(INSTALLMODE),native) 

    # 'native' install (choose ONE prefix)
PREFIX=/usr/local
#PREFIX=/usr
SYMLINK_PREFIX=
INCLUDEDIR=$(PREFIX)/include/intelib
INCLUDEDIR_SYMLINK=no
BINDIR=$(PREFIX)/bin
LIBDIR=$(PREFIX)/lib
#DOCDIR=$(PREFIX)/share/docs   # not used in this version

else

# 'separate directory' install
SYSPREFIX=/usr/local
PREFIX=$(SYSPREFIX)/intelib-$(VERSION_SUFFIX)
ifneq ($(findstring -$(VERSION_SUFFIX),$(PREFIX)),)
  SYMLINK_PREFIX=$(subst -$(VERSION_SUFFIX),,$(PREFIX))
else
  SYMLINK_PREFIX=$(PREFIX)
endif
INCLUDEDIR=$(PREFIX)/include
INCLUDEDIR_SYMLINK=yes
BINDIR=$(PREFIX)/bin
LIBDIR=$(PREFIX)/lib
#DOCDIR=$(PREFIX)/share/docs   # not used in this version

endif


INSTALL = install
INSTALL_DIR = $(INSTALL) -d
INSTALL_HEADERS = $(INSTALL) -m 0644
INSTALL_BIN = $(INSTALL) -m 0755
INSTALL_LIB = $(INSTALL) -m 0644
INSTALL_DATA = $(INSTALL) -m 0644


LDCONFIG = /sbin/ldconfig

ifneq ($(TARGETDIR),$(filter /%,$(TARGETDIR)))
TARGETDIRFP = $(CURDIR)/$(TARGETDIR)
else
TARGETDIRFP = $(TARGETDIR)
endif

default:	bootstrap
	:

bootstrap:
	$(MAKE) library USE_READLINE=$(USE_READLINE)
	cd ils && $(MAKE) bootstrap USE_READLINE=$(USE_READLINE)
	cd ill && $(MAKE) bootstrap USE_READLINE=$(USE_READLINE)
	[ -d irina ] && cd irina && $(MAKE)


library: $(TARGETDIRFP)
	ln -sf $(CURDIR)/sexpress $(TARGETDIRFP)
	ln -sf $(CURDIR)/tools $(TARGETDIRFP)
	ln -sf $(CURDIR)/genlisp $(TARGETDIRFP)
	ln -sf $(CURDIR)/scheme $(TARGETDIRFP)
	ln -sf $(CURDIR)/lisp $(TARGETDIRFP)
	ln -sf $(CURDIR)/interact $(TARGETDIRFP)
	ln -sf $(CURDIR)/ils $(TARGETDIRFP)
	ln -sf $(CURDIR)/ill $(TARGETDIRFP)
	ln -sf $(TARGETDIRFP) $(TARGETDIRFP)/intelib
	cd sexpress && $(MAKE) all TARGETDIR=$(TARGETDIRFP) \
				OPTIMIZATION=$(OPTIMIZATION) \
				TARGETLIBNAME=libintelib.a
	cd tools && $(MAKE) all_add TARGETDIR=$(TARGETDIRFP) \
				OPTIMIZATION=$(OPTIMIZATION) \
				TARGETLIBNAME=libintelib.a
	cd genlisp && $(MAKE) all_add TARGETDIR=$(TARGETDIRFP) \
				OPTIMIZATION=$(OPTIMIZATION) \
				TARGETLIBNAME=libintelib.a
	cd scheme && $(MAKE) all_add TARGETDIR=$(TARGETDIRFP) \
				OPTIMIZATION=$(OPTIMIZATION) \
				TARGETLIBNAME=libintelib.a
	cd lisp && $(MAKE) all_add TARGETDIR=$(TARGETDIRFP) \
				OPTIMIZATION=$(OPTIMIZATION) \
				TARGETLIBNAME=libintelib.a
	cd interact && $(MAKE) all TARGETDIR=$(TARGETDIRFP) \
				OPTIMIZATION=$(OPTIMIZATION) \
				USE_READLINE=$(USE_READLINE) \
				TARGETLIBNAME=libintelib_interp.a
	cd ils && $(MAKE) lib_add TARGETDIR=$(TARGETDIRFP) \
				OPTIMIZATION=$(OPTIMIZATION) \
				USE_READLINE=$(USE_READLINE) \
				TARGETLIBNAME=libintelib_interp.a
	cd ill && $(MAKE) lib_add TARGETDIR=$(TARGETDIRFP) \
				OPTIMIZATION=$(OPTIMIZATION) \
				USE_READLINE=$(USE_READLINE) \
				TARGETLIBNAME=libintelib_interp.a
	[ -d refal ] && cd refal && $(MAKE) all_add TARGETDIR=$(TARGETDIRFP) \
				OPTIMIZATION=$(OPTIMIZATION) \
				USE_READLINE=$(USE_READLINE) \
				TARGETLIBNAME=libintelib.a
ifeq ($(KEEP_OBJECTS),)
	rm $(TARGETDIR)/*.o
endif

$(TARGETDIRFP):	
	mkdir $(TARGETDIRFP)

version.h:	Version
	echo '#define INTELIB_VERSION "'`head -1 Version`'"' > $@
	echo '#define INTELIB_VERSIONID '`tail -1 Version` >> $@

doxydocs:
	doxygen docs/doxygen/intelib.conf


install:
		# as the interact/ directory presently doesn't hold files,
		# it is not to be mentioned here
	for D in sexpress tools genlisp lisp scheme ill ils refal ; do \
		$(INSTALL_DIR) $(DESTDIR)$(INCLUDEDIR)/$$D ; \
		$(INSTALL_HEADERS) $$D/*.hpp  $(DESTDIR)$(INCLUDEDIR)/$$D ; \
	done
	$(INSTALL_HEADERS) build/*.hpp $(DESTDIR)$(INCLUDEDIR)
	$(INSTALL_DIR) $(DESTDIR)$(LIBDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL_LIB) build/*.a $(DESTDIR)$(LIBDIR)
	$(INSTALL_BIN) ill/ill ill/nill $(DESTDIR)$(BINDIR)
	$(INSTALL_BIN) ils/ils ils/nils $(DESTDIR)$(BINDIR)
	$(INSTALL_DATA) ill/illdef._ls $(DESTDIR)$(BINDIR)
	$(INSTALL_DATA) ils/ilsdef._sc $(DESTDIR)$(BINDIR)
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


clean:
	cd sexpress && $(MAKE) clean
	cd tools && $(MAKE) clean
	cd genlisp && $(MAKE) clean
	cd lisp && $(MAKE) clean
	cd scheme && $(MAKE) clean
	cd ill && $(MAKE) clean
	cd ils && $(MAKE) clean
	cd samples && $(MAKE) clean
	cd tests && $(MAKE) clean
	cd refal && $(MAKE) clean || :
	cd irina && $(MAKE) clean || :
	rm -rf build docs/doxygen/html
	rm -rf build docs/doxygen/man
