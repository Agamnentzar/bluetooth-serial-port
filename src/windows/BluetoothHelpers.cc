#include <winsock2.h>
#include <ws2bth.h>
#include <string>
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

string BluetoothHelpers::GetWSAErrorMessage(int errorCode)
{
	LPTSTR buffer;

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&buffer, 0, NULL) == 0)
	{
		return "Unknown";
	}

	char buff[20];
	auto result = ToString(buffer);
	LocalFree(buffer);
	_itoa_s(errorCode, buff, sizeof(buff), 10);
	return string("(") + buff + string(") ") + result;
}

string BluetoothHelpers::ToString(LPTSTR str)
{
#ifdef UNICODE
	wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.to_bytes(wstring(str));
#else
	return string(str);
#endif
}
