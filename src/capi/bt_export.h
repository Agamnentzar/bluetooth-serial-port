#pragma once

#ifndef BLUETOOTH_C_DLL
#if defined(_MSC_VER)
#  define BLUETOOTH_C_DLL  __declspec(dllexport)
#  define BLUETOOTH_DLL  __declspec(dllexport)
#else
#  if defined(USE_GCC_VISIBILITY_FLAG)
#    define BLUETOOTH_C_DLL  __attribute__ ((visibility("default")))
#    define BLUETOOTH_DLL  __attribute__ ((visibility("default")))
#  else
#    define BLUETOOTH_C_DLL
#    define BLUETOOTH_DLL
#  endif
#endif

#ifdef __cplusplus
#  define BLUETOOTH_SERIAL_PORT_C_START  extern "C" {
#  define BLUETOOTH_SERIAL_PORT_C_END  }
#else
#  define BLUETOOTH_SERIAL_PORT_C_START
#  define BLUETOOTH_SERIAL_PORT_C_END
#endif

#endif