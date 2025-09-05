QEMU := qemu-system-i386
QEMU_ARGS := -drive format=raw,file=os.bin

.PHONY: all run clean

all: os.bin

boot/boot.bin:
	@$(MAKE) -C boot

kernel/kernel.bin:
	@$(MAKE) -C kernel

os.bin: boot/boot.bin kernel/kernel.bin
	dd if=boot/boot.bin of=$@ bs=512 count=1 status=none
	dd if=kernel/kernel.bin of=$@ bs=512 seek=1 count=2 status=none

floppy.img: os.bin
	dd if=/dev/zero of=floppy.img bs=512 count=2880
	dd if=os.bin of=floppy.img conv=notrunc

os.iso: os.bin
	mkdir -p iso
	cp os.bin iso/
	xorriso -as mkisofs -b os.bin -c boot.catalog -no-emul-boot -boot-load-size 4 -boot-info-table -o os.iso iso/

run: os.bin
	$(QEMU) $(QEMU_ARGS)

clean:
	@echo "RM os.bin"
	@rm -f os.bin
	@$(MAKE) -C boot clean
	@$(MAKE) -C kernel clean