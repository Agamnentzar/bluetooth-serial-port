# Multiplatform Bluetooth serial port communication library

Based on <a href="https://github.com/eelcocramer/node-bluetooth-serial-port">Bluetooth serial port communication for Node.js</a>

## Pre-requests on Linux

* CMake
* Needs Bluetooth development packages to build

`apt-get install build-essential libbluetooth-dev`

## Pre-request on OS X

* CMake from MacPorts
* Needs XCode and XCode command line tools installed.

## Pre-request on Windows

* CMake

# Documentation

## Basic usage

```cpp
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
```

## API

### DeviceINQ

#### DeviceINQ::Create()

Returns new instance of DeviceINQ object

#### DeviceINQ::Inquire()

Returns list of bluetooth devices in range

#### DeviceINQ::SdpSearch(address)

Returns channelID of device at given address

## LICENSE

This module is available under a [FreeBSD license](http://opensource.org/licenses/BSD-2-Clause), see the [LICENSE file](./LICENSE.md) for details.
