.PHONY: library

TARGETLIBNAME = libiprolog.a
LIBSOURCES = utils.cpp exceptions.cpp data.cpp engine.cpp

$(TARGETDIRFP)/$(TARGETLIBNAME): library
	cd library && $(MAKE)

MODULES=grammar.cpp io.cpp library.cpp output.cpp trace.cpp

GENERATED_PREFIX=plg

include ../lib.mk
