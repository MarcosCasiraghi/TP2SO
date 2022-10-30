include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_C=$(SOURCES:.c=.o)

all: $(MODULE)

$(MODULE): $(OBJECTS_ASM) $(OBJECTS_C)
	$(LD) -T sampleCodeModule.ld _loader.o $(OBJECTS_ASM) $(OBJECTS_C) -o ../$(MODULE)
	$(LD) -T sampleCodeModule.ld _loader.o $(OBJECTS_ASM) $(OBJECTS_C) --oformat=elf64-x86-64 -o ../0000-sampleCodeModule.elf

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c _loader.c $(SOURCES) 
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

#obs: aca se crea la carpeta obj si quisieramos
%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o

.PHONY: all clean print
