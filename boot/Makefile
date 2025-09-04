NASM=nasm
NASM_ARGS=-f bin 

all: boot.bin clean

boot.bin: src/main.asm
	@ mkdir -p build
	$(NASM) $(NASM_ARGS) $< -o $@

clean:
	rm -rf build

.PHONY: all clean