INTELIB=../..
EXAMPLES=example queens

.PHONY: default clean

default: $(EXAMPLES)

.cpp:
	g++ -O0 -g -I$(INTELIB) -L$(INTELIB)/build/intelib -o $@ $< -lintelib

clean:
	rm -f ${EXAMPLES}
