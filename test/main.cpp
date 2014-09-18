#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <exception>
#include <vector>
#include <ctime>
#include "../src/DeviceINQ.h"
#include "../src/Enums.h"
#include "../src/BluetoothException.h"

using namespace std;

string formatDate(const char *format, time_t time)
{
	if (time == 0)
		return "--";

	char buffer[256] = { 0 };
	tm *timeinfo = localtime(&time);
	strftime(buffer, sizeof(buffer), format, timeinfo);
	return buffer;
}

int main()
{
	try
	{
		DeviceINQ *inq = DeviceINQ::Create();
		vector<device> devices = inq->Inquire();

		for (auto it = devices.begin(); it != devices.end(); ++it)
		{
			cout << "\tname: " << it->name << endl;
			cout << "\taddress: " << it->address << endl;
			cout << "\tclass: " << GetDeviceClassString(it->deviceClass) << endl;
			cout << "\tmajor class: " << GetDeviceClassString(it->majorDeviceClass) << endl;
			cout << "\tservice class: " << GetServiceClassString(it->serviceClass) << endl;
			cout << "\tlast seen: " << formatDate("%c", it->lastSeen) << endl;
			cout << "\tlast used: " << formatDate("%c", it->lastUsed) << endl;
			cout << endl;
		}

		cout << endl << "done, found " << devices.size() << " device(s)" << endl;
	}
	catch (BluetoothException &e)
	{
		cout << e.what() << endl;
	}

	return 0;
}