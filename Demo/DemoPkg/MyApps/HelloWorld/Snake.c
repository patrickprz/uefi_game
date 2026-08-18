#include "Snake.h"
#include "Serial.h"

typedef enum{
  NONE,
  UP,
  DOWN,
  RIGHT,
  LEFT
} Direction;

typedef struct{
  UINT16 X;
  UINT16 Y;
  UINT16 Size;
  Direction Direction;
} Drawable;

UINTN GameSpeed = 250000;
UINT32 BorderColor = 0x00505050;
Drawable Player;
BOOLEAN Playing = TRUE;

VOID DrawBorder(VOID);
VOID DrawPlayer(VOID);
VOID DrawBackground(VOID);

EFI_STATUS ReadInputs(VOID);

EFI_STATUS
SnakeInit(VOID)
{
    return EFI_SUCCESS;
}


EFI_STATUS
SnakeGameLoop(VOID)
{
  Player.X = 120;
  Player.Y = 20;
  Player.Size = 20;
  Player.Direction = NONE;

  EFI_STATUS Status = EFI_SUCCESS;
  while(Playing)
  {
    ReadInputs();

    switch (Player.Direction)
    {
      case UP:
        Player.Y -= Player.Size;
        break;
      case DOWN:
        Player.Y += Player.Size;
        break;
      case RIGHT:
        Player.X += Player.Size;
        break;
      case LEFT:
        Player.X -= Player.Size;
        break;
      default:
        break;
    }

    DrawBackground();
    DrawBorder();
    DrawPlayer();
    GraphicsFlipBuffer();

    Status = gBS->Stall (GameSpeed);

    if(EFI_ERROR(Status)){
      return Status;
    }
  }
  return Status;
}


EFI_STATUS
SnakeClose(VOID){
    return EFI_SUCCESS;
}


EFI_STATUS
ReadInputs(VOID)
{
  EFI_INPUT_KEY Key;
  EFI_STATUS Status = EFI_SUCCESS;

  Key.ScanCode = 0;
  Key.UnicodeChar = '\0';

  gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);

  if (Key.ScanCode == SCAN_ESC){
    gST->RuntimeServices->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);
  }

  if (Key.ScanCode == SCAN_UP){
    Player.Direction = UP;
    CHAR8  *Message = "UP\r\n";
    Status = SerialWrite(Message);
    return EFI_SUCCESS;
  }

  if (Key.ScanCode == SCAN_DOWN){
    Player.Direction = DOWN;
    CHAR8  *Message = "DOWN\r\n";
    Status = SerialWrite(Message);
    return EFI_SUCCESS;
  }

  if (Key.ScanCode == SCAN_RIGHT){
    Player.Direction = RIGHT;
    CHAR8  *Message = "RIGHT\r\n";
    Status = SerialWrite(Message);
    return EFI_SUCCESS;
  }

  if (Key.ScanCode == SCAN_LEFT){
    Player.Direction = LEFT;
    CHAR8  *Message = "LEFT\r\n";
    Status = SerialWrite(Message);
    return EFI_SUCCESS;
  }

  if (Key.ScanCode == SCAN_ESC){
    Playing = FALSE;
    return EFI_SUCCESS;
  }

  if (Key.UnicodeChar == 's'){
    CHAR8  *Message = "SERIAL COMMAND\r\n";
    Status = SerialWrite(Message);
    return Status;
  }
  return EFI_NOT_READY;
}

VOID
DrawPlayer(VOID)
{
  DrawRectangle(Player.X, Player.X+Player.Size, Player.Y, Player.Y+Player.Size, 0X00FFFFFF);
}

VOID
DrawBackground(VOID)
{
  DrawRectangle(0, 100, 0, 800, 0x00000000);
  DrawRectangle(700, 800, 0, 800, 0x00000000);
  DrawRectangle(100, ScreenWidth-100, 0, 600, 0x202020);
}

VOID
DrawBorder(){
  UINT8 DrawablePositions = 600/20;
  Drawable Block;
  Block.X = 100;
  Block.Y = 0;
  Block.Size = 20;

  //Top
  for (UINT8 i = 0; i < DrawablePositions; i++)
  {
    DrawRectangle(Block.X, Block.X + Block.Size, Block.Y, Block.Y + Block.Size, BorderColor);
    Block.X += Block.Size;
  }

  //Bottom
  Block.X = 100;
  Block.Y = ScreenHeight-Block.Size;
  for (UINT8 i = 0; i < DrawablePositions; i++)
  {
    DrawRectangle(Block.X, Block.X + Block.Size, Block.Y, Block.Y + Block.Size, BorderColor);
    Block.X += Block.Size;
  }

  //Left
  Block.X = 100;
  Block.Y = Block.Size;
  for (UINT8 i = 0; i < DrawablePositions-1; i++)
  {
    DrawRectangle(Block.X, Block.X + Block.Size, Block.Y, Block.Y + Block.Size, BorderColor);
    Block.Y += Block.Size;
  }

  //Right
  Block.X = 680;
  Block.Y = Block.Size;
  for (UINT8 i = 0; i < DrawablePositions-1; i++)
  {
    DrawRectangle(Block.X, Block.X + Block.Size, Block.Y, Block.Y + Block.Size, BorderColor);
    Block.Y += Block.Size;
  }
}
