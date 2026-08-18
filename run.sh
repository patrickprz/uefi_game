#!/bin/sh

echo "Cleaning..."
rm -r root
mkdir -p root/efi/boot
echo "Starting..."
cp Demo/Build/DemoPkg/DEBUG_GCC/X64/HelloWorld.efi . 
mv HelloWorld.efi bootx64.efi
cp bootx64.efi root/efi/boot

qemu-system-x86_64 \
  -drive if=pflash,format=raw,file=./OVMF.fd \
  -drive format=raw,file=fat:rw:root \
  -net none \
#  -usb -usbdevice serial::vc
#  -enable-kvm
#  -nographic
