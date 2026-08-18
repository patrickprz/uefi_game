#include "KeyProcess.h"

EFI_INPUT_KEY 
KeyProcessorGetKey(VOID)
{
    EFI_EVENT Events[1];
    EFI_INPUT_KEY Key;

    Key.ScanCode = 0;
    Key.UnicodeChar = u'\0';

    Events[0] = gST->ConIn->WaitForKey;
    UINTN index = 0;
    gBS->WaitForEvent(1, Events, &index);

    if (index == 0) {
        gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
    }

    return Key;
}