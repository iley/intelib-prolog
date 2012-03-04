.PHONY: default clean
.SUFFIXES: .cpp .pro

MODULES=test queens deriv
CXXFLAGS+=-g -O0

default: $(MODULES)

$(MODULES): %: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -I../.. -L../../build/intelib -o $@ $< -lintelib

.pro.cpp: trans.pro
	prolog -q -s trans.pro -t "translate('$<')"

clean:
	rm -f $(MODULES) *.hpp *.cpp
