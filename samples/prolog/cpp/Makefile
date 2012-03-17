INTELIB=../../..
EXAMPLES=example queens psort dcg qsort
CXX=g++
CXXFLAGS+=-g -O2

.PHONY: default clean

default: $(EXAMPLES)

.cpp:
	$(CXX) $(CXXFLAGS) -I$(INTELIB) -L$(INTELIB)/build/intelib -o $@ $< -lintelib

clean:
	rm -f ${EXAMPLES}
