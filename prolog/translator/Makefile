.PHONY: default clean
.SUFFIXES: .cpp .pro

MODULES=test queens

default: $(MODULES)

$(MODULES): %: %.cpp %.hpp
	$(CXX) -I../.. -L../../build/intelib -o $@ $< -lintelib

.pro.cpp: trans.pro
	prolog -q -s trans.pro -- $<

clean:
	rm -f $(MODULES) *.hpp *.cpp
