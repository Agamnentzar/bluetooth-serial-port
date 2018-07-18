#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "BluetoothException.h"
#include "DeviceINQ.h"

extern "C"{
    #include <stdio.h>
    #include <errno.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <signal.h>
    #include <termios.h>
    #include <sys/poll.h>
    #include <sys/ioctl.h>
    #include <sys/socket.h>
    #include <assert.h>
    #include <time.h>
}

#import <Foundation/NSObject.h>
#import <IOBluetooth/objc/IOBluetoothDevice.h>
#import <IOBluetooth/objc/IOBluetoothRFCOMMChannel.h>
#import <IOBluetooth/objc/IOBluetoothSDPUUID.h>
#import <IOBluetooth/objc/IOBluetoothSDPServiceRecord.h>
#import "BluetoothWorker.h"

using namespace std;

DeviceINQ *DeviceINQ::Create()
{
	return new DeviceINQ();
}

DeviceINQ::DeviceINQ()
{
}

DeviceINQ::~DeviceINQ()
{
}

vector<device> DeviceINQ::Inquire(int length)
{
    (void)(length);
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    BluetoothWorker *worker = [BluetoothWorker getInstance];

    // create pipe to communicate with delegate
    pipe_t *pipe = pipe_new(sizeof(device_info_t), 0);
    [worker inquireWithPipe: pipe];
    pipe_consumer_t *c = pipe_consumer_new(pipe);
    pipe_free(pipe);

    device_info_t info;
    size_t result;
	vector<device> devices;

    do
	{
        result = pipe_pop_eager(c, &info, 1);

        if (result != 0)
		{
			int cod = info.classOfDevice;

			device dev;
			dev.address = string(info.address);
			dev.name = string(info.name);
			dev.connected = info.connected;
			dev.remembered = info.favorite;
			dev.authenticated = info.paired;
			dev.lastSeen = (uint32_t)info.lastSeen;
			dev.lastUsed = 0;
			dev.deviceClass = (DeviceClass)(cod & 0x1ffc);
			dev.majorDeviceClass = (DeviceClass)(cod & DC_Uncategorized);
			dev.serviceClass = (ServiceClass)(cod >> 13);
			devices.push_back(dev);
        }
    } while (result != 0);
    
    pipe_consumer_free(c);

    [pool release];
	return devices;
}

int DeviceINQ::SdpSearch(string address)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSString *addr = [NSString stringWithCString: address.c_str() encoding: NSASCIIStringEncoding];
    BluetoothWorker *worker = [BluetoothWorker getInstance];
    int channelID = [worker getRFCOMMChannelID: addr];

    [pool release];
	return channelID;
}
