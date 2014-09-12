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

Returns serial port channelID for device at given address

* __address__: string containing bluetooth address of the device

### BTSerialPortBinding

#### BTSerialPortBinding::Create(address, channelID)

Returns new instance of BTSerialPortBinding object

* __address__: string containint bluetooth address of the device
* __channelID__: ID of the serial port channel

#### BTSerialPortBinding::Connect()

Connects to the device, needs to be called before any Read/Write calls

#### BTSerialPortBinding::Close()

Closes connection to the device

#### BTSerialPortBinding::Read(buffer, length)

Reads data from the device, returns numbe rof bytes read

* __buffer__: pointer to buffer to hold received data
* __length__: maximum namber of bytes to read

#### BTSerialPortBinding::Write(buffer, length)

Writes data to the device

* __buffer__: pointer to buffer with data to send
* __length__: number of bytes to send

## LICENSE

This module is available under a [FreeBSD license](http://opensource.org/licenses/BSD-2-Clause), see the [LICENSE file](./LICENSE.md) for details.
