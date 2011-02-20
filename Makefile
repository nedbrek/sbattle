.PHONY: all clean test

BIN := sbattle.exe
SRC := class.cpp sim.cpp
OBJ := $(SRC:.cpp=.o)
DEP := $(SRC:.cpp=.d)

all: $(BIN)

-include $(DEP)

CXXFLAGS := -MP -MMD -Wall

.cpp.o:
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	@g++ -Wall -o $@ $^

test: $(BIN)
	@./$(BIN) --test > tmp
	diff tmp gold.out.txt

clean:
	@rm -f $(BIN) $(OBJ)

