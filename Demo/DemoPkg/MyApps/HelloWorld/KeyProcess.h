#ifndef KEYPROCESS_H
#define KEYPROCESS_H

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_INPUT_KEY KeyProcessorGetKey(VOID);

#endif //KEYPROCESS_H