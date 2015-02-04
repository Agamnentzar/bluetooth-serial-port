#include <stdio.h>
#include <stdlib.h>
#include "capi/bt_api.h"

int main( int argc, const char* argv[] )
{
  printf("Scanning\n");
  BTDeviceINQH device = BTDeviceINQ_Create();
  BTDeviceH* pDevices;
  uint64_t nDevices;

  if (BTDeviceINQ_Inquire(device, &pDevices, &nDevices) == RT_None)
  {
    printf("Discovered %llu devices\n", nDevices);
    for (uint64_t i = 0; i < nDevices; i++)
    {
      char* pszName = BTDevice_GetName(pDevices[i]);
      char* pszAddr = BTDevice_GetAddress(pDevices[i]);
      printf("Name: %s\n", pszName);
      printf("Address: %s\n", pszAddr);
      free(pszName);
    }
    BT_DestroyDevices(pDevices, nDevices);
  }
  else
    printf("Failed to find any devices");
}