#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <exception>
#include <vector>
#include <memory>
#include <ctime>
#include "../src/DeviceINQ.h"
#include "../src/Enums.h"
#include "../src/BluetoothException.h"

using namespace std;

string formatDate(const char *format, time_t time)
{
	if (time <= 0)
		return "--";

	char buffer[256] = { 0 };
	tm *timeinfo = localtime(&time);

	if (timeinfo)
		strftime(buffer, sizeof(buffer), format, timeinfo);

	return buffer;
}

int main()
{
	try
	{
		unique_ptr<DeviceINQ> inq(DeviceINQ::Create());
		vector<device> devices = inq->Inquire();

		for (const auto& d : devices)
		{
			cout << "\tname: " << d.name << endl;
			cout << "\taddress: " << d.address << endl;
			cout << "\tclass: " << GetDeviceClassString(d.deviceClass) << endl;
			cout << "\tmajor class: " << GetDeviceClassString(d.majorDeviceClass) << endl;
			cout << "\tservice class: " << GetServiceClassString(d.serviceClass) << endl;
			cout << "\tlast seen: " << formatDate("%c", d.lastSeen) << endl;
			cout << "\tlast used: " << formatDate("%c", d.lastUsed) << endl;
			cout << "\tchannel ID: " << inq->SdpSearch(d.address) << endl;
			cout << endl;
		}

		cout << endl << "done, found " << devices.size() << " device(s)" << endl;
	}
	catch (const BluetoothException& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}
