.PHONY: default

PREFIX=plg

OBJ=$(addprefix $(TARGETDIRFP)/$(PREFIX)_, $(MODULES:.cpp=.o))

all: $(OBJ)

$(TARGETDIRFP)/$(PREFIX)_%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
