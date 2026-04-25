#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SRC_DIR="$SCRIPT_DIR/../src"
OUT_DIR="$SCRIPT_DIR"
ISO_DIR="$SCRIPT_DIR/../iso"

nasm -f elf32 "$SRC_DIR/boot.asm" -o "$OUT_DIR/boot.o"

g++ -m32 -c "../src/kernel/main.cpp" -o "$OUT_DIR/kernel.o" -ffreestanding -nostdlib -fno-stack-protector -fno-exceptions -fno-rtti -lgcc

ld -m elf_i386 -T "$SRC_DIR/linker.ld" "$OUT_DIR/boot.o" "$OUT_DIR/kernel.o" -o "$OUT_DIR/kernel.bin"

cp "$OUT_DIR/kernel.bin" "$ISO_DIR/boot/"
grub-mkrescue -o "$OUT_DIR/os.iso" "$ISO_DIR"

qemu-system-i386 -cdrom "$OUT_DIR/os.iso"
