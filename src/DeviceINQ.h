#ifndef BLUETOOTH_DEVICE_INQ_H
#define BLUETOOTH_DEVICE_INQ_H

#include <vector>
#include <string>

struct device
{
	std::string address;
	std::string name;
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
	std::vector<device> Inquire();
	int SdpSearch(std::string address);
};

#endif