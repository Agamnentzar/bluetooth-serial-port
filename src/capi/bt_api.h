#ifndef BLUETOOTH_SERIAL_PORT_API_H_INCLUDED
#define BLUETOOTH_SERIAL_PORT_API_H_INCLUDED

#define BLUETOOTH_SERIAL_PORT_C_API 1

#include "bt_config.h"

BLUETOOTH_SERIAL_PORT_C_START

BLUETOOTH_C_DLL BTDeviceINQH BTDeviceINQ_Create();
BLUETOOTH_C_DLL RTError BTDeviceINQ_Inquire(BTDeviceINQH hDeviceINQ, BTDeviceH** hDevices, uint64_t* nDevices);
BLUETOOTH_C_DLL RTError BTDeviceINQ_SdpSearch(BTDeviceINQH hDeviceINQ, const char* addr, int* channel_id);
BLUETOOTH_C_DLL void BTDeviceINQ_Destroy(BTDeviceINQH hDeviceINQ);

BLUETOOTH_C_DLL BTSerialPortBindingH  BTSerialPortBinding_Create(const char* addr, int channel_id);
BLUETOOTH_C_DLL RTError BTSerialPortBinding_Connect(BTSerialPortBindingH hBinding);
BLUETOOTH_C_DLL RTError BTSerialPortBinding_Close(BTSerialPortBindingH hBinding);
BLUETOOTH_C_DLL RTError BTSerialPortBinding_Read(BTSerialPortBindingH hBinding, char* buffer, int length);
BLUETOOTH_C_DLL RTError BTSerialPortBinding_Write(BTSerialPortBindingH hBinding, const char *buffer, int length);
BLUETOOTH_C_DLL void BTSerialPortBinding_Destroy(BTSerialPortBindingH hBinding);

BLUETOOTH_C_DLL void BT_Free(void* object);

BLUETOOTH_C_DLL char* BTDevice_GetName(BTDeviceH hDevice);
BLUETOOTH_C_DLL char* BTDevice_GetAddress(BTDeviceH hDevice);
BLUETOOTH_C_DLL int BTDevice_IsConnected(BTDeviceH hDevice);
BLUETOOTH_C_DLL int BTDevice_IsAuthenticated(BTDeviceH hDevice);

BLUETOOTH_C_DLL void Error_Reset(void);
BLUETOOTH_C_DLL char* Error_GetLastErrorMsg(void);
BLUETOOTH_C_DLL int Error_GetErrorCount(void);
BLUETOOTH_C_DLL int Error_GetLastErrorNum(void);
BLUETOOTH_C_DLL char* Error_GetLastErrorMethod(void);

BLUETOOTH_C_DLL void BT_DestroyDevices(BTDeviceH* hDevices, uint32_t nDevices);

BLUETOOTH_C_DLL char* BT_Version();

BLUETOOTH_SERIAL_PORT_C_END

#endif