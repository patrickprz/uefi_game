#include "Usb.h"

EFI_STATUS
UsbInit(EFI_HANDLE _ImageHandle){
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN NoSerialProtocol = 0;
  EFI_HANDLE *HandleBuffer = NULL;
  EFI_USB_IO_PROTOCOL *UsbIoProtocol = NULL;  
  USB_DEVICE_DESCRIPTOR UsbDeviceDescriptor;


  gBS->LocateHandleBuffer(ByProtocol,
                        &gEfiUsbIoProtocolGuid,
                        NULL,
                        &NoSerialProtocol,
                        &HandleBuffer
                        );


  Print (L"%d USB Devices:\n", NoSerialProtocol);
    
  for (UINTN i = 0; i < NoSerialProtocol; i++)
  {
    gBS->OpenProtocol(HandleBuffer[i],
                      &gEfiUsbIoProtocolGuid,
                      (VOID **)&UsbIoProtocol,
                      _ImageHandle,
                      NULL,
                      EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    
    UsbIoProtocol->UsbGetDeviceDescriptor(UsbIoProtocol, &UsbDeviceDescriptor);

    Print(L"Vendor id: %d Device Id %d \r\n\r\n", UsbDeviceDescriptor.IdVendor, 
          UsbDeviceDescriptor.IdProduct);

    gBS->CloseProtocol(HandleBuffer[i],
                      &gEfiUsbIoProtocolGuid,
                      _ImageHandle,
                      NULL);
  }

  if(HandleBuffer != NULL){
    gBS->FreePool(HandleBuffer);
  }
  
  gBS->Stall(5000000);

  return Status;
}