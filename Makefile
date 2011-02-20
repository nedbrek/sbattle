.PHONY: all clean

BIN := sbattle.exe
SRC := class.cpp sim.cpp
OBJ := $(SRC:.cpp=.o)
DEP := $(SRC:.cpp=.d)

-include $(DEP)

CXXFLAGS := -MP -MMD -Wall

all: $(BIN)

.cpp.o:
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	@g++ -Wall -o $@ $^

clean:
	@rm -f $(BIN) $(OBJ)

