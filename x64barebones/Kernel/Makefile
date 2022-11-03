include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c memoryManager/*.c interruptions/*.c library/*.c pipe/*.c semaphore/*.c scheduling/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)


OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

LOADERSRC=loader.asm

MM=-D FREE_MANAGER

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) 
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $^
	$(LD) $(LDFLAGS) -T kernel.ld -o $@ $^


%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include $(MM) -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin *.elf */*.o

.PHONY: all clean
