#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

EFI_STATUS GraphicsInit (UINT16 TargetWidth, UINT16 TargetHeight);
EFI_STATUS GraphicsGetResolution (VOID);
VOID DrawRectangle(UINT16 x1, UINT16 x2, UINT16 y1, UINT16 y2, UINT32 Color);
VOID ClearScreen(UINT32 Color);
VOID GraphicsFlipBuffer(VOID);
VOID GraphicsEnd(VOID);

extern UINT16 ScreenWidth;
extern UINT16 ScreenHeight;
#endif //GRAPHICS_H
