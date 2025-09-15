#!/bin/bash

sudo qemu-system-x86_64 \
  -smp 4 \
  -kernel ./arch/x86_64/boot/bzImage \
  -nographic \
  -drive file=rootfs.img,media=disk,format=raw \
  -drive file=mydisk.img,media=disk,format=qcow2 \
  -drive file=/mnt/ramdisk/my_disk.img,media=disk,format=raw \
  -append "console=ttyS0 nokaslr root=/dev/sda rw" \
  -m 32G \
  --enable-kvm \
  -netdev user,id=net0,hostfwd=tcp::2222-:22 \
  -serial mon:stdio \
  -device e1000,netdev=net0 
# Flags required for gdb. Uncomment and then connect
# gdb to linux using 
# gdb ./vmlinux
# target remote localhost:1234
#\
#  -s \
#  -S
