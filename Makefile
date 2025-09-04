QEMU := qemu-system-i386
QEMU_ARGS := -drive format=raw,file=os.bin

.PHONY: all run clean

all: os.bin

boot/boot.bin:
	@$(MAKE) -C boot

kernel/kernel.bin:
	@$(MAKE) -C kernel

os.bin: boot/boot.bin kernel/kernel.bin
	@echo "DD $@"
	@dd if=boot/boot.bin of=$@ bs=512 count=1 status=none
	@dd if=kernel/kernel.bin of=$@ bs=512 seek=1 count=2 status=none

run: os.bin
	$(QEMU) $(QEMU_ARGS)

clean:
	@echo "RM os.bin"
	@rm -f os.bin
	@$(MAKE) -C boot clean
	@$(MAKE) -C kernel clean