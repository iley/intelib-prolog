INTELIB=../../..
#EXAMPLES=example queens psort dcg qsort
#FIXME
EXAMPLES=example
CXX=g++
CXXFLAGS+=-g -O0

.PHONY: default clean intelib build

default: intelib build

build: $(EXAMPLES)

intelib:
	$(MAKE) -C $(INTELIB) library

$(EXAMPLES): %: %.cpp intelib
	$(CXX) $(CXXFLAGS) -I$(INTELIB) -L$(INTELIB)/build/intelib -o $@ $< -lintelib

clean:
	rm -f ${EXAMPLES}
