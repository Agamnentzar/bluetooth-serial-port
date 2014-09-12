#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS


#include <windows.h>
#include <string>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2bth.h>
#include "../BluetoothException.h"
#include "../DeviceINQ.h"
#include "BluetoothHelpers.h"

using namespace std;

DeviceINQ *DeviceINQ::Create()
{
	auto inquire = new DeviceINQ();

	if (!inquire->initialized)
	{
		delete inquire;
		throw BluetoothException("Unable to initialize socket library");
	}

	return inquire;
}

DeviceINQ::DeviceINQ()
{
	initialized = BluetoothHelpers::Initialize();
}

DeviceINQ::~DeviceINQ()
{
	if (initialized)
		BluetoothHelpers::Finalize();
}

vector<device> DeviceINQ::Inquire()
{
	// Construct windows socket bluetooth variables
	DWORD flags = LUP_CONTAINERS | LUP_FLUSHCACHE | LUP_RETURN_NAME | LUP_RETURN_ADDR;
	DWORD querySetSize = sizeof(WSAQUERYSET);
	auto querySet = (WSAQUERYSET *)malloc(querySetSize);

	if (querySet == nullptr)
		throw BluetoothException("Out of memory: Unable to allocate memory resource for inquiry");

	ZeroMemory(querySet, querySetSize);
	querySet->dwSize = querySetSize;
	querySet->dwNameSpace = NS_BTH;

	// Initiate client device inquiry
	HANDLE lookupServiceHandle;
	int lookupServiceError = WSALookupServiceBegin(querySet, flags, &lookupServiceHandle);
	vector<device> devices;

	if (lookupServiceError != SOCKET_ERROR)
	{
		// Iterate over each found bluetooth service
		bool inquiryComplete = false;

		while (!inquiryComplete)
		{
			// For each bluetooth service retrieve its corresponding details
			lookupServiceError = WSALookupServiceNext(lookupServiceHandle, flags, &querySetSize, querySet);

			if (lookupServiceError != SOCKET_ERROR)
			{
				char address[40] = { 0 };
				DWORD addressLength = _countof(address);
				SOCKADDR_BTH *bluetoothSocketAddress = (SOCKADDR_BTH *)querySet->lpcsaBuffer->RemoteAddr.lpSockaddr;
				BTH_ADDR bluetoothAddress = bluetoothSocketAddress->btAddr;

				// Emit the corresponding event if we were able to retrieve the address
				int addressToStringError = WSAAddressToString(
					querySet->lpcsaBuffer->RemoteAddr.lpSockaddr, sizeof(SOCKADDR_BTH), nullptr, address, &addressLength);

				if (addressToStringError != SOCKET_ERROR)
				{
					// Strip any leading and trailing parentheses is encountered
					char strippedAddress[19] = { 0 };
					auto sscanfResult = sscanf(address, "(" "%18[^)]" ")", strippedAddress);
					auto addressString = sscanfResult == 1 ? std::string(strippedAddress) : std::string(address);

					device d;
					d.address = addressString;
					d.name = std::string(querySet->lpszServiceInstanceName);
					devices.push_back(d);
				}
			}
			else
			{
				int lookupServiceErrorNumber = WSAGetLastError();

				if (lookupServiceErrorNumber == WSAEFAULT)
				{
					free(querySet);
					querySet = (WSAQUERYSET *)malloc(querySetSize);

					if (querySet == nullptr)
					{
						WSALookupServiceEnd(lookupServiceHandle);
						throw BluetoothException("Out of memory: Unable to allocate memory resource for inquiry");
					}
				}
				else if (lookupServiceErrorNumber == WSA_E_NO_MORE)
				{
					// No more services where found
					inquiryComplete = true;
				}
				else
				{
					// Unhandled error
					inquiryComplete = true;
				}
			}
		}
	}
	else if (WSAGetLastError() != WSASERVICE_NOT_FOUND)
	{
		free(querySet);
		throw BluetoothException("Unable to initiate client device inquiry");
	}

	free(querySet);
	WSALookupServiceEnd(lookupServiceHandle);
	
	return devices;
}

int DeviceINQ::SdpSearch(string address)
{
	// Construct windows socket bluetooth variables
	DWORD flags = LUP_FLUSHCACHE | LUP_RETURN_ADDR;
	DWORD querySetSize = sizeof(WSAQUERYSET);
	auto querySet = (WSAQUERYSET *)malloc(querySetSize);

	if (querySet == nullptr)
		throw BluetoothException("Out of memory: Unable to allocate memory resource for sdp search");

	char addressBuffer[40];

	if (strcpy_s(addressBuffer, address.c_str()) != 0)
		throw BluetoothException("Address (first argument) length is invalid");

	ZeroMemory(querySet, querySetSize);
	querySet->dwSize = querySetSize;
	querySet->dwNameSpace = NS_BTH;
	querySet->lpServiceClassId = (LPGUID)&SerialPortServiceClass_UUID;
	querySet->dwNumberOfCsAddrs = 0;
	querySet->lpszContext = addressBuffer;

	// Initiate client device inquiry
	HANDLE lookupServiceHandle;
	int lookupServiceError = WSALookupServiceBegin(querySet, flags, &lookupServiceHandle);
	int channelID = -1;

	if (lookupServiceError != SOCKET_ERROR)
	{
		// Iterate over each found bluetooth service
		bool inquiryComplete = false;

		while (!inquiryComplete)
		{
			// For each bluetooth service retrieve its corresponding details
			lookupServiceError = WSALookupServiceNext(lookupServiceHandle, flags, &querySetSize, querySet);

			if (lookupServiceError != SOCKET_ERROR)
			{
				char address[19] = { 0 };
				SOCKADDR_BTH *bluetoothSocketAddress = (SOCKADDR_BTH *)querySet->lpcsaBuffer->RemoteAddr.lpSockaddr;
				channelID = bluetoothSocketAddress->port;
				inquiryComplete = true;
			}
			else
			{
				int lookupServiceErrorNumber = WSAGetLastError();

				if (lookupServiceErrorNumber == WSAEFAULT)
				{
					free(querySet);
					querySet = (WSAQUERYSET *)malloc(querySetSize);

					if (querySet == nullptr)
					{
						WSALookupServiceEnd(lookupServiceHandle);
						throw BluetoothException("Out of memory: Unable to allocate memory resource for sdp search");
					}
				}
				else if (lookupServiceErrorNumber == WSA_E_NO_MORE)
				{
					// No more services where found
					inquiryComplete = true;
				}
				else
				{
					// Unhandled error
					inquiryComplete = true;
				}
			}
		}
	}
	else if (WSAGetLastError() != WSASERVICE_NOT_FOUND)
	{
		free(querySet);
		throw BluetoothException("WSA service not found");
	}

	free(querySet);
	WSALookupServiceEnd(lookupServiceHandle);

	return channelID;
}