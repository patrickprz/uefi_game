#include "Serial.h"

EFI_SERIAL_IO_PROTOCOL *SerialIO = NULL;
EFI_HANDLE *HandleBuffer = NULL;

EFI_STATUS
SerialInit(EFI_HANDLE _ImageHandler)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN NoHandles = 0;

    Status = gBS->LocateHandleBuffer (
                                        ByProtocol,
                                        &gEfiSerialIoProtocolGuid,
                                        NULL,
                                        &NoHandles,
                                        &HandleBuffer
    );

    if (EFI_ERROR (Status) || NoHandles == 0){
        Print(L"Any serial device %d \r\n", NoHandles);
        return Status;
    }

    Print(L"Serial device found %d \r\n", NoHandles);

    Status = gBS->OpenProtocol (
                    HandleBuffer[0],
                    &gEfiSerialIoProtocolGuid,
                    (VOID **)&SerialIO,
                    _ImageHandler,
                    NULL,
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL
                    );

    if (EFI_ERROR (Status)) {
        Print (L"Error opening serial device: %r\n", Status);
        if (HandleBuffer != NULL) {
            Status = gBS->FreePool (HandleBuffer);
        }
        return Status;
    }

    Status = SerialIO->SetAttributes (
                        SerialIO,
                        115200,                  // BaudRate
                        0,                       // ReceiveFifoDepth (0 = usa o padrao do hardware)
                        1000000,                 // Timeout (em microsegundos = 1s)
                        DefaultParity,           // Paridade (NoParity / DefaultParity)
                        8,                       // DataBits
                        DefaultStopBits          // StopBits (OneStopBit / DefaultStopBits)
                        );

    if (EFI_ERROR (Status)) {
        Print (L"Aviso: Falha ao configurar atributos da porta (%r). Continuando com padrao...\n", Status);
    }

    return Status;
}   

EFI_STATUS
SerialWrite(CHAR8 *Data)
{   
    EFI_STATUS Status = EFI_SUCCESS;

    UINTN  BufferSize = AsciiStrLen (Data);

    Status = SerialIO->Write (
                        SerialIO,
                        &BufferSize,
                        (VOID *)Data
                        );

    return Status;
}

EFI_STATUS
SerialClose(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;
    if (HandleBuffer != NULL) {
        Status = gBS->FreePool (HandleBuffer);
    }
    return Status;
}