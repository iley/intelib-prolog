INTELIB=../../..
EXAMPLES=test queens deriv dcg
CXX=g++
CXXFLAGS+=-g -O2

.PHONY: default clean
.SUFFIXES: .cpp .pro

default: $(EXAMPLES)

$(EXAMPLES): %: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -I$(INTELIB) -L$(INTELIB)/build/intelib -o $@ $< -lintelib

.pro.cpp: trans.pro
	prolog -qs trans.pro -t "translate('$<')"

clean:
	rm -f $(EXAMPLES) *.hpp *.cpp
