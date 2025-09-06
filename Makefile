QEMU := qemu-system-i386
QEMU_ARGS := -fda floppy.img

.PHONY: all run clean

all: floppy.img

boot/boot.bin:
	@$(MAKE) -C boot

kernel/kernel.bin:
	@$(MAKE) -C kernel

os.bin: boot/boot.bin kernel/kernel.bin
	dd if=boot/boot.bin of=$@ bs=512 count=1 status=none
	dd if=kernel/kernel.bin of=$@ bs=512 seek=1 count=17 status=none

floppy.img: os.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=os.bin of=$@ conv=notrunc

run: floppy.img
	$(QEMU) $(QEMU_ARGS)

clean:
	@echo "RM os.bin"
	@rm -f os.bin
	@echo "RM floppy.img"
	@rm -f floppy.img
	@$(MAKE) -C boot clean
	@$(MAKE) -C kernel clean