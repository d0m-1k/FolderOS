CC := gcc
LD := ld
CFLAGS := -m32 -ffreestanding -fno-pic -fno-pie -nostdlib -Iinclude -fno-stack-protector -Wall -Wextra -mno-sse -mno-mmx -mno-3dnow -mno-avx -mno-80387
LDFLAGS := -m elf_i386 -T linker.ld --oformat=binary
NASM := nasm
NASMFLAGS := -f elf32 -g -Wall

BUILD_DIR := build
SRC_DIR := src

C_SRCS := $(wildcard ${SRC_DIR}/*.c)
ASM_SRCS := $(wildcard ${SRC_DIR}/*.asm)
C_OBJS := $(C_SRCS:${SRC_DIR}/%.c=${BUILD_DIR}/%.o)
ASM_OBJS := $(ASM_SRCS:${SRC_DIR}/%.asm=${BUILD_DIR}/%.o)
OBJS := ${C_OBJS} ${ASM_OBJS}

.PHONY: all clean

all: kernel.bin

kernel.bin: ${OBJS} linker.ld | ${BUILD_DIR}
	@echo "Линковка ядра..."
	@${LD} ${LDFLAGS} ${OBJS} -o $@
	@echo "Размер ядра: $$(stat -c%s $@) байт"

${BUILD_DIR}/%.o: ${SRC_DIR}/%.c | ${BUILD_DIR}
	@echo "Компиляция $<..."
	@${CC} ${CFLAGS} -c $< -o $@

${BUILD_DIR}/%.o: ${SRC_DIR}/%.asm | ${BUILD_DIR}
	@echo "Ассемблирование $<..."
	@${NASM} ${NASMFLAGS} $< -o $@

${BUILD_DIR}:
	@mkdir -p $@

clean:
	@echo "Очистка ядра..."
	@rm -rf ${BUILD_DIR} kernel.bin