export PATH=$PATH:/usr/local/i386elfgcc/bin

nasm "Bootloader/boot.asm" -f bin -o "Binaries/boot.bin"
nasm "Kernel/kernel_entry.asm" -f elf -o "Binaries/kernel_entry.o"
i386-elf-gcc -ffreestanding -m32 -g -c "Kernel/kernel.cpp" -o "Binaries/kernel.o"
i386-elf-gcc -ffreestanding -m32 -g -c "Include/system.cpp" -o "Binaries/system.o"
i386-elf-gcc -ffreestanding -m32 -g -c "Include/screen.cpp" -o "Binaries/screen.o"
i386-elf-gcc -ffreestanding -m32 -g -c "Include/idt.cpp" -o "Binaries/idt.o"
i386-elf-gcc -ffreestanding -m32 -g -c "Include/isrs.cpp" -o "Binaries/isrs.o"
nasm "Kernel/zeroes.asm" -f bin -o "Binaries/zeroes.bin"

i386-elf-ld -o "Binaries/full_kernel.bin" -Ttext 0x1000 "Binaries/kernel_entry.o" "Binaries/kernel.o" "Binaries/system.o" "Binaries/screen.o" "Binaries/idt.o" "Binaries/isrs.o" --oformat binary

cat "Binaries/boot.bin" "Binaries/full_kernel.bin" "Binaries/zeroes.bin"  > "Binaries/OS.bin"
qemu-system-x86_64 -drive format=raw,file=Binaries/OS.bin,index=0,if=floppy,  -m 128M