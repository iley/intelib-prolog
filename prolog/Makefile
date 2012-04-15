.PHONY: library

TARGETLIBNAME = libiprolog.a
LIBSOURCES = plgutils.cpp plgexceptions.cpp plgdata.cpp plgengine.cpp plgpackage.cpp plgcontext.cpp

$(TARGETDIRFP)/$(TARGETLIBNAME): library
	cd library && $(MAKE)

MODULES=grammar.cpp io.cpp library.cpp output.cpp trace.cpp

GENERATED_PREFIX=plg

include ../lib.mk
