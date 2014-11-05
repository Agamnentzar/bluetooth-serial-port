#include <winsock2.h>
#include <ws2bth.h>
#include "BluetoothHelpers.h"

using namespace std;

bool BluetoothHelpers::Initialize()
{
	WSADATA data;
	int startupError = WSAStartup(MAKEWORD(2, 2), &data);
	bool initializationSuccess = startupError == 0;

	if (initializationSuccess)
	{
		initializationSuccess = LOBYTE(data.wVersion) == 2 && HIBYTE(data.wVersion) == 2;

		if (!initializationSuccess)
			BluetoothHelpers::Finalize();
	}

	return initializationSuccess;
}

void BluetoothHelpers::Finalize()
{
	WSACleanup();
}

string BluetoothHelpers::GetWSAErrorMessage()
{
	LPTSTR buffer;
	int errorCode = WSAGetLastError();

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&buffer, 0, NULL) == 0)
	{
		return "Unknown";
	}

	char buff[20];
	auto result = string(buffer);
	LocalFree(buffer);
	_itoa_s(errorCode, buff, sizeof(buff), 10);
	return string("(") + buff + string(") ") + result;
}