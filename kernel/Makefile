GCC=gcc
GCC_ARGS=-c -m32 -ffreestanding -fno-pic -fno-pie -nostdlib -Iinclude

LD=ld
LD_ARGS=-m elf_i386 -T linker.ld --oformat=binary

all: kernel.bin clean

build/kernel.o: src/kernel.c
	@ mkdir -p build
	$(GCC) $(GCC_ARGS) -o $@ $<

kernel.bin: build/kernel.o
	$(LD) $(LD_ARGS) $^ -o $@

clean:
	rm -rf build

.PHONY: all clean