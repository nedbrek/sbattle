.PHONY: all clean

all: sbattle.exe

sbattle.exe: class.o sim.o
	@g++ -Wall -o $@ $^

clean:
	@rm -f sbattle.exe *.o

