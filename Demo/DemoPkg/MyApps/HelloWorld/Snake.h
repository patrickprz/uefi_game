#ifndef SNAKE_H
#define SNAKE_H
#include "Uefi.h"
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include "Graphics.h"

EFI_STATUS SnakeGameLoop(VOID);
EFI_STATUS SnakeInit(VOID);
EFI_STATUS SnakeClose(VOID);
#endif //SNAKE_H