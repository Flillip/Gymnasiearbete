GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = bin/loader.o bin/kernel.o

bin/%.o: %.cpp
	mkdir -p bin
	g++ $(GPPPARAMS) -o $@ -c $<

bin/%.o: %.s
	mkdir -p bin
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	mkdir -p bin
	ld $(LDPARAMS) -T $< -o bin/$@ $(objects)

mykernel.iso: mykernel.bin
	mkdir -p bin
	mkdir -p bin/iso/boot/grub
	cp bin/$< bin/iso/boot
	@echo 'set timeout=0' 					>  bin/iso/boot/grub/grub.cfg
	@echo 'set default=0' 					>> bin/iso/boot/grub/grub.cfg
	@echo 'menuentry "My OS" {' 			>> bin/iso/boot/grub/grub.cfg
	@echo '  multiboot /boot/mykernel.bin' 	>> bin/iso/boot/grub/grub.cfg
	@echo '  boot' 							>> bin/iso/boot/grub/grub.cfg
	@echo '}' 								>> bin/iso/boot/grub/grub.cfg

	grub-mkrescue --output=bin/$@ ./bin/iso
	rm -rf bin/iso

run: mykernel.iso
	(killall qemu-system-x86_64 && sleep 1) || true
	qemu-system-x86_64 -boot d -cdrom ./bin/mykernel.iso -m 512 &