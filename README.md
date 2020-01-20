# Multiplatform Bluetooth serial port communication library

Based on [Bluetooth serial port communication for Node.js](https://github.com/eelcocramer/node-bluetooth-serial-port)

## Prequisites on Linux

* CMake
* Needs Bluetooth development packages to build

`apt-get install libbluetooth-dev cmake gcc-c++`  
`zypper install bluez-devel cmake gcc-c++`

## Prequisites on OS X

* CMake from MacPorts
* Needs XCode and XCode command line tools installed.

## Prequisites on Windows

* CMake
* Visual Studio

# Documentation

## Basic usage

```cpp
#include <iostream>
#include <vector>
#include <memory>
#include "../src/DeviceINQ.h"

using namespace std;

void main()
{
	unique_ptr<DeviceINQ> inq(DeviceINQ::Create());
	vector<device> devices = inq->Inquire();

	for (const auto& d : devices)
	{
		cout << d.name << " " << d.address << endl;
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

```cpp
struct device
{
	string address; // bluetooth address of the device
	string name; // name of the device
	time_t lastSeen; // last time device was seen in the inquiry (windows, osx)
	time_t lastUsed; // last time device was used (windows)
	bool connected; // true if device is connected (windows, osx)
	bool remembered; // true if device is remembered (windows, osx)
	bool authenticated; // true if device is authenticated (windows, osx)
	DeviceClass deviceClass; // class of device
	DeviceClass majorDeviceClass; // major class of device
	ServiceClass serviceClass; // service class flags
};
```

*Note for Mac users:* `DeviceINQ` relies on a separate executable named `btScan`
built together with the library which has to be in the same directory as the
library (the code searching for it is at the top of `DeviceINQ::Inquire()` and
can be modified according to your needs).  The reason for this is that
`IOBluetoothDeviceInquiry` has the undocumented requirement that the RunLoop of
the application's main thread be executed.  While almost any GUI application will
fulfill this requirement, a simple command line tool such as the bundled example
won't.  Since we can't just hijack the application's main thread, this is the only
way to guarantee that the device search functions correctly.

#### DeviceINQ::SdpSearch(address)

Returns serial port channelID for device at given address

* __address__: string containing bluetooth address of the device

```
Note: This method seems to fail on Windows, use value 1 for channelID instead
```

### BTSerialPortBinding

#### BTSerialPortBinding::Create(address, channelID)

Returns new instance of BTSerialPortBinding object

* __address__: string containint bluetooth address of the device
* __channelID__: ID of the serial port channel

```
Note: channelID should be always 1 for serial port
```

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

#### BTSerialPortBinding::IsDataAvailable(buffer, length)

Returns true if there is data in the buffer ready to be read
(not implemented for OSX - always returns false)

### Other

#### GetDeviceClassString(deviceClass)

Returns text representation of deviceClass enum value

#### GetServiceClassString(serviceClass)

Returns text representation of serviceClass enum value

## LICENSE

This module is available under a [FreeBSD license](http://opensource.org/licenses/BSD-2-Clause), see the [LICENSE file](./LICENSE.md) for details.
