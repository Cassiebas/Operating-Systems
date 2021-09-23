CFLAGS:= -fno-pie -ffreestanding

.PHONY: all build clean

build/rapi-boot/eos-init.bin: build/obj/init.o build/obj/dev-gpio.o
	aarch64-linux-gnu-gcc -o eos-init.elf -nostdlib -fno-pie -nostartfiles -ffreestanding -T linker.ld -e start build/obj/init.o build/obj/dev-gpio.o
	aarch64-linux-gnu-objcopy eos-init.elf -O binary -j .eos-* build/rapi-boot/eos-init.bin
	rm eos-init.elf

build/obj/init.o: src/init.c src/eos.h
	aarch64-linux-gnu-gcc -o $@ -c $(CFLAGS) $<

build/obj/dev-gpio.o: src/dev-gpio.c src/eos.h
	aarch64-linux-gnu-gcc -o $@ -c $(CFLAGS) $<

build: build/rapi-boot/eos-init.bin
	dd if=/dev/zero of=build/eos.img count=100 bs=1M
	sudo losetup -D
	sudo losetup -fP build/eos.img
	printf "o\nn\np\n1\n\n\nt\nc\nw\n" | sudo fdisk /dev/loop0
	sudo losetup -o 1048576 /dev/loop1 /dev/loop0
	sudo mkfs.vfat -n EOS /dev/loop1
	sudo mount /dev/loop1 build/mnt
	sudo cp -r build/rapi-boot/* build/mnt/.
	sudo umount build/mnt
	sudo losetup -D

clean:
	if [ -f "build/rapi-boot/eos-init.bin" ]; then rm build/rapi-boot/eos-init.bin; fi
	if [ -f "build/eos.img" ]; then rm build/eos.img; fi
	if [ -f "build/obj/init.o" ]; then rm build/obj/*.o; fi
	if [ -f "build/obj/dev-gpio.o" ]; then rm build/obj/*.o; fi