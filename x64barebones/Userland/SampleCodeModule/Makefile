include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

all: $(MODULE)

$(MODULE): $(SOURCES) $(OBJECTS_ASM)
	$(GCC) $(GCCFLAGS) -I./include -T sampleCodeModule.ld _loader.c $(OBJECTS_ASM) $(SOURCES) -o ../$(MODULE)

#obs: aca se crea la carpeta obj si quisieramos
%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o

.PHONY: all clean print
