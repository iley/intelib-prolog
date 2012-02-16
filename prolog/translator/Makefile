.PHONY: clean

test: test.cpp test.hpp
	g++ -I../.. -L../../build/intelib -o $@ $< -lintelib

test.hpp: test.cpp

test.cpp: test.pro
	./trans.pro $<

clean:
	rm -f test test.hpp test.cpp
