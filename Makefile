CC=gcc
CFLAGS=-O2 -Wall
SOURCES=src/main.c
HEADERS=src/opcodes.h
EXECUTABLE=scooma

all: $(EXECUTABLE) image.bin
	
$(EXECUTABLE): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

image.bin: runtime.sasm
	python asm.py runtime.sasm

clean:
	rm fvm image.bin
