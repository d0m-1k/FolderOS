QEMU=qemu-system-i386
QEMU_ARGS=

all: os.bin

boot/boot.bin:
	make -C boot

os.bin: boot/boot.bin
	cat $^ > $@

run: os.bin
	$(QEMU) -drive format=raw,file=$<

.PHONY: all run