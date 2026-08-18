#include "Graphics.h"

EFI_GRAPHICS_OUTPUT_PROTOCOL  *Gop = NULL;
UINT32 *FrameBuffer = NULL;
UINT32 *BackBuffer = NULL;
UINT16 ScreenWidth = 0;
UINT16 ScreenHeight = 0;
UINTN BackBufferSize = 0;

EFI_STATUS GraphicsSetResolution (UINT16 TargetWidth, UINT16 TargetHeight);
  // Aloca o buffer
EFI_STATUS
GraphicsInit(UINT16 TargetWidth, UINT16 TargetHeight)
{

    EFI_STATUS Status = EFI_SUCCESS;
    
    Status = gBS->LocateProtocol (
                    &gEfiGraphicsOutputProtocolGuid,
                    NULL,
                    (VOID **)&Gop
                    );


    FrameBuffer = (UINT32 *)Gop->Mode->FrameBufferBase;
    ScreenWidth = Gop->Mode->Info->PixelsPerScanLine;
    ScreenHeight = Gop->Mode->Info->VerticalResolution;

    GraphicsSetResolution(TargetWidth, TargetHeight);
    gBS->Stall (1000000);
    return Status;
}

EFI_STATUS
GraphicsGetResolution ()
{
  EFI_STATUS Status = EFI_SUCCESS;

  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
  UINTN SizeOfInfo;

  for (UINT32 ModeIndex = 0; ModeIndex < Gop->Mode->MaxMode; ModeIndex++) {
    
    EFI_STATUS Status = Gop->QueryMode (Gop, ModeIndex, &SizeOfInfo, &Info);
    
    if (!EFI_ERROR (Status)) {
      Print (
        L"Modo %d: %d x %d\n", 
        ModeIndex, 
        Info->HorizontalResolution, 
        Info->VerticalResolution
      );
    }
  }
  return Status;
}

EFI_STATUS 
GraphicsSetResolution(UINT16 TargetWidth, UINT16 TargetHeight)
{
    EFI_STATUS Status = EFI_SUCCESS;

    UINT32                        ModeIndex;
    UINT32                        MaxMode;
    UINTN                         SizeOfInfo;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
    BOOLEAN                       ModeFound = FALSE;


    if (EFI_ERROR (Status)) {
        Print (L"Erro: Nao foi possivel localizar o GOP: %r\n", Status);
        return Status;
    }

    MaxMode = Gop->Mode->MaxMode;

    // 2. Procura entre os modos suportados a resolucao desejada
    for (ModeIndex = 0; ModeIndex < MaxMode; ModeIndex++) {
        Status = Gop->QueryMode (Gop, ModeIndex, &SizeOfInfo, &Info);
        if (EFI_ERROR (Status)) {
        continue;
        }

        Print (L"Modo %d: %d x %d\n", ModeIndex, Info->HorizontalResolution, Info->VerticalResolution);

        // Verifica se é a resolução desejada
        if (Info->HorizontalResolution == TargetWidth && Info->VerticalResolution == TargetHeight) {
        ModeFound = TRUE;
        break;
        }
    }

    // 3. Aplica a nova resolução se foi encontrada
    if (ModeFound) {
        Status = Gop->SetMode (Gop, ModeIndex);
        if (EFI_ERROR (Status)) {
          Print (L"Erro ao definir a resolucao para o modo %d: %r\n", ModeIndex, Status);
        } else {
          Print (L"Resolucao alterada com sucesso para %d x %d!\n", TargetWidth, TargetHeight);
          ScreenWidth = Gop->Mode->Info->PixelsPerScanLine;
          ScreenHeight = Gop->Mode->Info->VerticalResolution;
        }
    } else {
        Print (L"A resolucao %d x %d nao eh suportada por esta GPU/Monitor.\n", TargetWidth, TargetHeight);
        Status = EFI_UNSUPPORTED;
    }


    BackBufferSize = Gop->Mode->FrameBufferSize;
    if (BackBufferSize < (800 * 600 * sizeof(UINT32))) {
      BackBufferSize = 800 * 600 * sizeof(UINT32);
    }

    UINTN Pages;
    Pages = EFI_SIZE_TO_PAGES (BackBufferSize) + 1; // +1 página de segurança para afastar a Guard Page
    BackBuffer = (UINT32 *) AllocatePages (Pages);

    // BackBuffer = (UINT32 *)(UINTN)AllocateZeroPool(BackBufferSize);
    if (BackBuffer == NULL) {
      Print (L"Erro ao alocar %d bytes para o buffer da tela: %r\n", BackBufferSize, Status);
      Status = EFI_OUT_OF_RESOURCES;
    }
    Print (L"BackBuffer alocado %r\r\n", Status);

    return Status;
}

VOID
ClearScreen(UINT32 Color)
{
  DrawRectangle(0, ScreenWidth, 0, ScreenHeight, Color);
}


VOID
DrawRectangle(UINT16 x1, UINT16 x2, UINT16 y1, UINT16 y2, UINT32 Color)
{
  UINT16 RectangleWidth = x2 - x1;
  UINT16 RectangleHeight = y2 - y1;

  if(x2> ScreenWidth || y2 > ScreenHeight){
    return;
  }

  for(UINT16 i = y1; i < y1 + RectangleHeight; i++){
    UINT32 LineOffset = i * ScreenWidth;
    for(UINT16 j = x1; j < x1 + RectangleWidth; j++){
      BackBuffer[LineOffset + j] = Color;
    }
  }
}

VOID 
GraphicsFlipBuffer(VOID){
  if (Gop != NULL && BackBuffer != NULL) {
      // Copia todo o conteúdo do BackBuffer de uma só vez para o VRAM (Main Framebuffer)
      CopyMem (
        (VOID *)FrameBuffer,
        (VOID *)BackBuffer,
        BackBufferSize
        );
    }
    ZeroMem (BackBuffer, BackBufferSize);
}

VOID
GraphicsEnd(VOID){
  if (BackBuffer != NULL) {
    gBS->FreePool (BackBuffer);
    BackBuffer = NULL;
  }
  Print (L"Buffer liberado com sucesso.\n");
}