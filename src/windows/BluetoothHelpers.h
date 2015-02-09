#pragma once

#include <string>

class BluetoothHelpers
{
public:
	static bool Initialize();
	static void Finalize();
	static std::string GetWSAErrorMessage(int errorCode);
	static std::string ToString(LPTSTR str);
};
