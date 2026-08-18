#ifndef SERIAL_H
#define SERIAL_H

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SerialIo.h>


EFI_STATUS SerialInit(EFI_HANDLE _ImageHandler);
EFI_STATUS SerialWrite(CHAR8 *Data);
EFI_STATUS SerialClose(VOID);

#endif // SERIAL_H