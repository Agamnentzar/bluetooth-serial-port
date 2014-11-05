#pragma once

#include <string>

class BluetoothHelpers
{
public:
	static bool Initialize();
	static void Finalize();
	static std::string GetWSAErrorMessage();
};
