include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_TESTS=$(wildcard tests/*.c)
SOURCES_LIBRARY=$(wildcard library/*.c)
SOURCES_PROGRAMS=$(wildcard programs/*.c)

OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_C=$(SOURCES:.c=.o)
OBJECTS_TESTS=$(SOURCES_TESTS:.c=.o)
OBJECTS_LIBRARY=$(SOURCES_LIBRARY:.c=.o)
OBJECTS_PROGRAMS=$(SOURCES_PROGRAMS:.c=.o)

all: $(MODULE)

$(MODULE): $(OBJECTS_ASM) $(OBJECTS_C) $(OBJECTS_LIBRARY) $(OBJECTS_PROGRAMS) $(OBJECTS_TESTS)
	$(LD) -T sampleCodeModule.ld _loader.o $(OBJECTS_ASM) $(OBJECTS_C) $(OBJECTS_LIBRARY) $(OBJECTS_PROGRAMS) $(OBJECTS_TESTS) -o ../$(MODULE)
	$(LD) -T sampleCodeModule.ld _loader.o $(OBJECTS_ASM) $(OBJECTS_C) $(OBJECTS_LIBRARY) $(OBJECTS_PROGRAMS) $(OBJECTS_TESTS) --oformat=elf64-x86-64 -o ../0000-sampleCodeModule.elf

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c _loader.c $(SOURCES) 
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

#obs: aca se crea la carpeta obj si quisieramos
%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o asm/*.o tests/*.o programs/*.o library/*.o

.PHONY: all clean print
