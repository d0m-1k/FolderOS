NASM := nasm
NASMFLAGS := -f bin -I src/

SRCS := $(wildcard src/*.asm)

.PHONY: all clean

all: build boot.bin

boot.bin: src/main.asm $(SRCS)
	@echo "NASM $@"
	@$(NASM) $(NASMFLAGS) $< -o $@

build:
	@mkdir -p $@

clean:
	@echo "RM build"
	@rm -rf build
	@echo "RM boot.bin"
	@rm -rf boot.bin

.PHONY: all clean