GPPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
		  obj/gdt.o \
		  obj/drivers/driver.o \
		  obj/hardwarecommunication/port.o \
		  obj/hardwarecommunication/interruptstubs.o \
		  obj/hardwarecommunication/interrupts.o \
		  obj/drivers/keyboard.o \
		  obj/drivers/mouse.o \
		  obj/kernel.o

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(GPPPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D)
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

.PHONY: clean
clean:
	rm -f obj bin/mykernel.bin bin/mykernel.iso