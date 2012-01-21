INTELIB=../..

example: example.cpp
	g++ -g -I$(INTELIB) -L$(INTELIB)/build/intelib -o $@ $< -lintelib

clean:
	rm -f example
