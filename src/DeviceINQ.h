#pragma once

#include "Enums.h"
#include <vector>
#include <string>
#include <ctime>

struct device
{
	std::string address;
	std::string name;
	std::time_t lastSeen;
	std::time_t lastUsed;
	bool connected;
	bool remembered;
	bool authenticated;
	DeviceClass deviceClass;
	DeviceClass majorDeviceClass;
	ServiceClass serviceClass;
};

class DeviceINQ
{
private:
#ifdef _WINDOWS_
	bool initialized;
#endif
	DeviceINQ();

public:
	~DeviceINQ();
	static DeviceINQ *Create();
	std::vector<device> Inquire(int length = 8);
	int SdpSearch(std::string address);
};
