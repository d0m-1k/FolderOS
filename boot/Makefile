NASM := nasm
NASMFLAGS := -f bin -I src/ -Wall
BUILD_DIR := build

SRCS := $(wildcard src/*.asm)
OBJS := $(SRCS:src/%.asm=${BUILD_DIR}/%.o)

.PHONY: all clean

all: boot.bin

boot.bin: src/main.asm ${SRCS} | ${BUILD_DIR}
	@echo "Компиляция загрузчика..."
	@${NASM} ${NASMFLAGS} $< -o $@
	@if [ $$(stat -c%s $@) -gt 512 ]; then \
		echo "Ошибка: Загрузчик превышает 512 байт"; \
		exit 1; \
	fi
	@echo "Размер загрузчика: $$(stat -c%s $@) байт"

${BUILD_DIR}:
	@mkdir -p $@

clean:
	@echo "Очистка загрузчика..."
	@rm -rf ${BUILD_DIR} boot.bin