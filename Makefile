QEMU := qemu-system-i386
QEMU_ARGS := -fda floppy.img -serial stdio
KERNEL_SECTORS := 17
FLOPPY_SECTORS := 2880

.PHONY: all run clean debug boot kernel

all: floppy.img

boot:
	@$(MAKE) -C boot all

kernel:
	@$(MAKE) -C kernel all

os.bin: boot kernel
	@echo "Создание os.bin..."
	@dd if=/dev/zero of=$@ bs=512 count=1 2>/dev/null
	@dd if=boot/boot.bin of=$@ bs=512 count=1 conv=notrunc status=none
	@dd if=kernel/kernel.bin of=$@ bs=512 count=${KERNEL_SECTORS} seek=1 conv=notrunc status=none
	@echo "Размер os.bin: $$(stat -c%s $@) байт"

floppy.img: os.bin
	@echo "Создание floppy.img..."
	@dd if=/dev/zero of=$@ bs=512 count=${FLOPPY_SECTORS} status=none
	@dd if=os.bin of=$@ conv=notrunc status=none
	@echo "Образ создан: $@"

run: floppy.img
	@echo "Запуск QEMU..."
	${QEMU} ${QEMU_ARGS}

debug: floppy.img
	@echo "Запуск QEMU в режиме отладки..."
	${QEMU} ${QEMU_ARGS} -s -S &

clean:
	@echo "Очистка..."
	@rm -f os.bin floppy.img
	@$(MAKE) -C boot clean
	@$(MAKE) -C kernel clean

check:
	@which nasm >/dev/null || echo "Ошибка: NASM не установлен"
	@which gcc >/dev/null || echo "Ошибка: Компилятор C не установлен"
	@which ${QEMU} >/dev/null || echo "Ошибка: QEMU не установлен"