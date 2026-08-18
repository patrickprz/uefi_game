#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include "Serial.h"
#include "Graphics.h"
#include "KeyProcess.h"
#include "Snake.h"

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  Status = gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
  Status = gST->ConOut->ClearScreen(gST->ConOut);

  // UsbInit(ImageHandle);

  GraphicsInit(800, 600);  
  SerialInit(ImageHandle);
  SnakeInit();

  SnakeGameLoop();

  SnakeClose();
  SerialClose();
  GraphicsEnd();

  gST->RuntimeServices->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);

  return Status;
}