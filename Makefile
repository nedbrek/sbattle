.PHONY: all clean lib test

BIN := sbattle.exe
LIB := sbattle.so
SRC := class.cpp ship.cpp sim.cpp
OBJ := $(SRC:.cpp=.o)
DEP := $(SRC:.cpp=.d)

THIRDI := /c/Tcl/include
THIRDL := /c/Tcl/lib/tcl85.lib

all: $(BIN)

lib: $(LIB)

-include $(DEP)

CXXFLAGS := -MP -MMD -Wall -I$(THIRDI) -g
LDFLAGS := -Wall

.cpp.o:
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN): $(OBJ) main.o
	@g++ $(LDFLAGS) -o $@ $^

$(LIB): $(OBJ) tclmain.o
	@g++ $(LDFLAGS) -shared -o $@ sbattle.def $^ $(THIRDL)

test: $(BIN)
	@./$(BIN) --test > tmp
	diff tmp gold.out.txt

clean:
	@rm -f $(BIN) $(OBJ)

