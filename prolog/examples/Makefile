INTELIB=../..
EXAMPLES=example queens psort dcg
CXX=g++

.PHONY: default clean

default: $(EXAMPLES)

.cpp:
	$(CXX) -O2 -g -I$(INTELIB) -L$(INTELIB)/build/intelib -o $@ $< -lintelib

clean:
	rm -f ${EXAMPLES}
