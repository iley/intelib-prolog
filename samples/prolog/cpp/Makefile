INTELIB=../../..
EXAMPLES=example queens psort dcg qsort
CXX=g++
CXXFLAGS+=-g -O2

.PHONY: default clean intelib build

default: intelib build

build: $(EXAMPLES)

intelib:
	$(MAKE) -C $(INTELIB) library

$(EXAMPLES): %: %.cpp intelib
	$(CXX) $(CXXFLAGS) -I$(INTELIB) -L$(INTELIB)/build/intelib -o $@ $< -lintelib

clean:
	rm -f ${EXAMPLES}
