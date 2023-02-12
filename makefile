SRC = $(wildcard src/*.cpp )
OUT = main.exe

CPP = g++

CFLAGS = -Isrc/include/ -Lsrc/Library/ -lsdl2 -lsdl2main -lSDL2_ttf

all: $(SRC)
	$(CPP)  $(SRC) -o $(OUT) $(CFLAGS)
