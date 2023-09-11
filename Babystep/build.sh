nasm ./src/boot.asm -f bin -o ./boot.bin
qemu-system-i386 -hda boot.bin