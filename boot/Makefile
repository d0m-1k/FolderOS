NASM=nasm
NASM_ARGS=-f bin 

all: boot.bin clean

boot.bin: src/*.asm
	@ mkdir -p build
	$(NASM) $(NASM_ARGS) src/main.asm -o $@

clean:
	rm -rf build

.PHONY: all clean