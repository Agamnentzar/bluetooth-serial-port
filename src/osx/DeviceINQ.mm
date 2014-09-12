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

DeviceINQ::DeviceINQ() {

}
    
DeviceINQ::~DeviceINQ() {

}
 
vector<device> DeviceINQ::Inquire() {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    BluetoothWorker *worker = [BluetoothWorker getInstance];
	vector<device> devices;

    // create pipe to communicate with delegate
    pipe_t *pipe = pipe_new(sizeof(device_info_t), 0);
    [worker inquireWithPipe: pipe];
    pipe_consumer_t *c = pipe_consumer_new(pipe);
    pipe_free(pipe);

    device_info_t *info = new device_info_t;
    size_t result;

    do {
        result = pipe_pop_eager(c, info, 1);

        if (result != 0) {
			device d;
			d.address = string(info->address);
			d.name = string(info->name);
			devices.push_back(d);
        }
    } while (result != 0);
    
    delete info;
    pipe_consumer_free(c);

    [pool release];
	return devices;
}
    
int DeviceINQ::SdpSearch(string address) {
	char addressBuffer[40];

	if (address.length() >= 40)
		throw BluetoothException("Address length is invalid");

	strcpy(addressBuffer, address.c_str());

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    NSString *addr = [NSString stringWithCString:addressBuffer encoding:NSASCIIStringEncoding];
    BluetoothWorker *worker = [BluetoothWorker getInstance];
    int channelID = [worker getRFCOMMChannelID: addr];

    [pool release];
	return channelID;
}