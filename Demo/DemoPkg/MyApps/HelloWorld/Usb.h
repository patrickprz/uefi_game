#ifndef USB_H
#define USB_h

#include "Uefi.h"
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiUsbLib.h>
#include "Graphics.h"

EFI_STATUS UsbInit(EFI_HANDLE _ImageHandle);
#endif //USB_H

