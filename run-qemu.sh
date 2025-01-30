#!/bin/bash

sudo qemu-system-x86_64 \
  -kernel ./arch/x86_64/boot/bzImage \
  -nographic \
  -drive file=rootfs.img,media=disk,format=raw \
  -drive file=mydisk.img,media=disk,format=qcow2 \
  -append "console=ttyS0 nokaslr root=/dev/sda rw" \
  -m 32G \
  --enable-kvm \
  -netdev user,id=net0,hostfwd=tcp::2222-:22 \
  -serial mon:stdio \
  -device e1000,netdev=net0
