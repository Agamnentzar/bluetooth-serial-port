#ifndef BLUETOOTH_SERIAL_PORT_CONFIG_H_INCLUDED
#define BLUETOOTH_SERIAL_PORT_CONFIG_H_INCLUDED

#ifdef _MSC_VER

#if _MSC_VER <= 1500
  typedef unsigned __int64 uint64_t;
#endif

   #include <windows.h>
   #define STRDUP _strdup

#else

   #include <stdint.h>
   #define STRDUP strdup
#endif

#include <sys/stat.h>

#include "capi/bt_export.h"
#include "DeviceINQ.h"

class BTSerialPortBinding;
class DeviceINQ;

typedef enum
{
   RT_None = 0,
   RT_Debug = 1,
   RT_Warning = 2,
   RT_Failure = 3,
   RT_Fatal = 4
} RTError;

typedef BTSerialPortBinding *BTSerialPortBindingH;
typedef DeviceINQ *BTDeviceINQH;
typedef device *BTDeviceH;
#endif