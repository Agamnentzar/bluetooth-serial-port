#include <iostream>
#include <vector>
#include "../src/DeviceINQ.h"

using namespace std;

void main()
{
	DeviceINQ *inq = DeviceINQ::Create();
	vector<device> devices = inq->Inquire();

	for (auto it = devices.begin(); it != devices.end(); ++it)
	{
		cout << it->name << " " << it->address << endl;
	}

	cout << endl << "done, found " << devices.size() << " device(s)" << endl;
}