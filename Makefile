QEMU=qemu-system-i386
QEMU_ARGS=

all: os.bin

boot/boot.bin: boot/src/*
	make -C boot

kernel/kernel.bin: kernel/src/*
	make -C kernel

os.bin: boot/boot.bin kernel/kernel.bin
	dd if=boot/boot.bin of=os.bin bs=512 count=1
	dd if=kernel/kernel.bin of=os.bin bs=512 seek=1 count=1

run: os.bin
	$(QEMU) -drive format=raw,file=$<

clean:
	rm -f os.img
	rm -f kernel/kernel.bin
	rm -f boot/boot.bin

.PHONY: all run clean