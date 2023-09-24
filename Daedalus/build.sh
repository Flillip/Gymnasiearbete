#!/bin/sh
# nasm ./boot.asm -f bin -o ./boot.bin
# qemu-system-i386 -hda boot.bin -monitor stdio 

mkdir -p ./bin
cd ./bin

# compile
nasm "../boot.asm" -f bin -o "boot.bin"
nasm "../kernel_entry.asm" -f elf -o "kernel_entry.o"
nasm "../zeroes.asm" -f bin -o "zeroes.bin"

# assemble and link
i386-elf-gcc -ffreestanding -m32 -g -c "../kernel.cpp" -o "kernel.o"
i386-elf-ld -o "full_kernel.bin" -Ttext 0x1000 "kernel_entry.o" "kernel.o" --oformat binary

# combine
cat "boot.bin" "full_kernel.bin" > "everything.bin"
cat "everything.bin" "zeroes.bin" > "OS.bin"

qemu-system-x86_64 -drive format=raw,file="OS.bin",index=0,if=floppy, -m 128M -monitor stdio 

cd ..