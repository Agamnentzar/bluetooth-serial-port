#include "capi/bt_impl.h"
#include "BluetoothException.h"

static std::stack<Error> errors;

#define VALIDATE_POINTER0(ptr, func) \
  do { if( NULL == ptr ) { \
    RTError const ret = RT_Failure; \
    std::ostringstream msg; \
    msg << "Pointer \'" << #ptr << "\' is NULL in \'" << (func) <<"\'."; \
    std::string message(msg.str()); \
    Error_PushError( ret, message.c_str(), (func)); \
    return; \
  }} while(0)

#define VALIDATE_POINTER1(ptr, func, rc) \
  do { if( NULL == ptr ) { \
    RTError const ret = RT_Failure; \
    std::ostringstream msg; \
    msg << "Pointer \'" << #ptr << "\' is NULL in \'" << (func) <<"\'."; \
    std::string message(msg.str()); \
    Error_PushError( ret, message.c_str(), (func)); \
    return (rc); \
  }} while(0)

BLUETOOTH_SERIAL_PORT_C_START

BLUETOOTH_C_DLL void Error_Reset(void) {
  if (errors.empty()) return;
  for (std::size_t i=0;i<errors.size();i++) errors.pop();
}

BLUETOOTH_C_DLL void Error_Pop(void) {
  if (errors.empty()) return;
  errors.pop();
}

BLUETOOTH_C_DLL int Error_GetLastErrorNum(void){
  if (errors.empty())
    return 0;
  else {
    Error err = errors.top();
    return err.GetCode();
  }
}

BLUETOOTH_C_DLL char* Error_GetLastErrorMsg(void){
  if (errors.empty())
    return NULL;
  else {
    Error err = errors.top();
    return STRDUP(err.GetMessage());
  }
}

BLUETOOTH_C_DLL char* Error_GetLastErrorMethod(void){
  if (errors.empty())
    return NULL;
  else {
    Error err = errors.top();
    return STRDUP(err.GetMethod());
  }
}

BLUETOOTH_C_DLL void Error_PushError(int code, const char *message, const char *method) {
  Error err = Error(code, std::string(message), std::string(method));
  errors.push(err);
}

BLUETOOTH_C_DLL int Error_GetErrorCount(void) {
  return static_cast<int>(errors.size());
}

BLUETOOTH_C_DLL BTDeviceINQH BTDeviceINQ_Create()
{
  return (BTDeviceINQH) DeviceINQ::Create();
}

BLUETOOTH_C_DLL RTError BTDeviceINQ_Inquire(BTDeviceINQH hDeviceINQ, BTDeviceH** hDevices, uint64_t* nDevices)
{
  VALIDATE_POINTER1(hDeviceINQ, "BTDeviceINQ_Inquire", RT_Failure);
  DeviceINQ* pInq = static_cast<DeviceINQ*>(hDeviceINQ);
  try
  {
    std::vector<device> v = pInq->Inquire();
    *nDevices = v.size();
    if (*nDevices > 0)
    {
      *hDevices = (device**)malloc(*nDevices * sizeof(device*));
      for (int i = 0; i < *nDevices; i++)
      {
        *hDevices[i] = &v[i];
      }      
    }
    else
    {
      *hDevices = NULL;
    }
  }
  catch(BluetoothException &e) 
  {
    Error_PushError(RT_Failure,
            e.what(),
            "BTDeviceINQ_Inquire");
    return RT_Failure;
  }
  return RT_None;
}

BLUETOOTH_C_DLL RTError BTDeviceINQ_SdpSearch(BTDeviceINQH hDeviceINQ, const char* addr, int* channel_id)
{
  VALIDATE_POINTER1(hDeviceINQ, "BTDeviceINQ_SdpSearch", RT_Failure);
  DeviceINQ* pInq = static_cast<DeviceINQ*>(hDeviceINQ);
  try
  {
    *channel_id = pInq->SdpSearch(addr);
  }
  catch(BluetoothException &e)
  {
    Error_PushError(RT_Failure,
            e.what(),
            "BTDeviceINQ_SdpSearch");
    return RT_Failure;    
  }
  return RT_None;
}

BLUETOOTH_C_DLL void BTDeviceINQ_Destroy(BTDeviceINQH hDeviceINQ)
{
  VALIDATE_POINTER0(hDeviceINQ, "BTDeviceINQ_Destroy");
  DeviceINQ* pInq = static_cast<DeviceINQ*>(hDeviceINQ);
  if (pInq) delete pInq;
}

BLUETOOTH_C_DLL BTSerialPortBindingH BTSerialPortBinding_Create(const char* addr, int channel_id)
{
  return (BTSerialPortBindingH) BTSerialPortBinding::Create(addr, channel_id);
}

BLUETOOTH_C_DLL RTError BTSerialPortBinding_Connect(BTSerialPortBindingH hBinding)
{
  VALIDATE_POINTER1(hBinding, "BTSerialPortBinding_Connect", RT_Failure);
  BTSerialPortBinding* pBinding = static_cast<BTSerialPortBinding*>(hBinding);
  try
  {
    pBinding->Connect();
  }
  catch(BluetoothException &e)
  {
    Error_PushError(RT_Failure,
            e.what(),
            "BTSerialPortBinding_Connect");
    return RT_Failure;
  }
  return RT_None;
}

BLUETOOTH_C_DLL RTError BTSerialPortBinding_Close(BTSerialPortBindingH hBinding)
{
  VALIDATE_POINTER1(hBinding, "BTSerialPortBinding_Close", RT_Failure);
  BTSerialPortBinding* pBinding = static_cast<BTSerialPortBinding*>(hBinding);
  try
  {
    pBinding->Close();
  }
  catch(BluetoothException &e)
  {
    Error_PushError(RT_Failure,
            e.what(),
            "BTSerialPortBinding_Close");
    return RT_Failure;
  }
  return RT_None;  
}

BLUETOOTH_C_DLL RTError BTSerialPortBinding_Read(BTSerialPortBindingH hBinding, char* buffer, int length)
{
  VALIDATE_POINTER1(hBinding, "BTSerialPortBinding_Read", RT_Failure);
  BTSerialPortBinding* pBinding = static_cast<BTSerialPortBinding*>(hBinding);
  try
  {
    pBinding->Read(buffer, length);
  }
  catch(BluetoothException &e)
  {
    Error_PushError(RT_Failure,
            e.what(),
            "BTSerialPortBinding_Read");
    return RT_Failure;
  }
  return RT_None; 
}

BLUETOOTH_C_DLL RTError BTSerialPortBinding_Write(BTSerialPortBindingH hBinding, const char *buffer, int length)
{
  VALIDATE_POINTER1(hBinding, "BTSerialPortBinding_Write", RT_Failure);
  BTSerialPortBinding* pBinding = static_cast<BTSerialPortBinding*>(hBinding);
  try
  {
    pBinding->Write(buffer, length);
  }
  catch(BluetoothException &e)
  {
    Error_PushError(RT_Failure,
            e.what(),
            "BTSerialPortBinding_Write");
    return RT_Failure;
  }
  return RT_None; 
}

BLUETOOTH_C_DLL void BTSerialPortBinding_Destroy(BTSerialPortBindingH hBinding)
{
  VALIDATE_POINTER0(hBinding, "BTSerialPortBinding_Destroy");
  BTSerialPortBinding* pBinding = static_cast<BTSerialPortBinding*>(hBinding);
  if (pBinding) delete pBinding;
}

BLUETOOTH_C_DLL void BT_Free(void* object)
{
  VALIDATE_POINTER0(object, "BT_Free");
  if (object != 0)
      std::free(object);
}

BLUETOOTH_C_DLL char* BTDevice_GetName(BTDeviceH hDevice)
{
  VALIDATE_POINTER1(hDevice, "BTDevice_GetName", 0);
  device* pDev = static_cast<device*>(hDevice);
  return STRDUP(pDev->name.c_str());
}

BLUETOOTH_C_DLL char* BTDevice_GetAddress(BTDeviceH hDevice)
{
  VALIDATE_POINTER1(hDevice, "BTDevice_GetAddress", 0);
  device* pDev = static_cast<device*>(hDevice);
  return STRDUP(pDev->address.c_str());
}

BLUETOOTH_C_DLL int BTDevice_IsConnected(BTDeviceH hDevice)
{
  VALIDATE_POINTER1(hDevice, "BTDevice_IsConnected", 0);
  device* pDev = static_cast<device*>(hDevice);
  return pDev->connected;
}

BLUETOOTH_C_DLL int BTDevice_IsAuthenticated(BTDeviceH hDevice)
{
  VALIDATE_POINTER1(hDevice, "BTDevice_IsAuthenticated", 0);
  device* pDev = static_cast<device*>(hDevice);
  return pDev->authenticated;
}

BLUETOOTH_C_DLL void BT_DestroyDevices(BTDeviceH* hDevices, uint32_t nDevices)
{
  VALIDATE_POINTER0(hDevices, "BT_DestroyDevices");
  // don't need to destroy actual devices they are returned as vector from
  // DeviceINQ::Inquire()
  delete hDevices;
}

BLUETOOTH_C_DLL char* BT_Version()
{
	std::ostringstream ot;

#ifdef BLUETOOTH_SERIAL_PORT_RELEASE_NAME
	ot << BLUETOOTH_SERIAL_PORT_RELEASE_NAME;
#else
	ot << "0.0.1";
#endif
	std::string out(ot.str());
	return STRDUP(out.c_str());
}
BLUETOOTH_SERIAL_PORT_C_END

