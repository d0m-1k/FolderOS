CC := cc
LD := ld
CFLAGS := -m32 -ffreestanding -fno-pic -fno-pie -nostdlib -Iinclude -fno-stack-protector -Wall -Wextra -mno-sse -mno-mmx -mno-3dnow -mno-avx -mno-80387
LDFLAGS := -m elf_i386 -T linker.ld --oformat=binary

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=build/%.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: build kernel.bin

kernel.bin: $(OBJS)
	@echo "LD $@"
	@$(LD) $(LDFLAGS) $^ -o $@

build/%.o: src/%.c
	@echo "CC $@"
	@$(CC) $(CFLAGS) -c $< -o $@

build:
	@mkdir -p $@

clean:
	@echo "RM build"
	@rm -rf build
	@echo "RM kernel.bin"
	@rm -rf kernel.bin

-include $(DEPS)