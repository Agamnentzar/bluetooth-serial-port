#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#import <Foundation/NSObject.h>
#import <IOBluetooth/objc/IOBluetoothDevice.h>
#import <IOBluetooth/objc/IOBluetoothDeviceInquiry.h>
#import "pipe.h"
#include "BluetoothException.h"
#include "BTSerialPortBinding.h"
#include "BluetoothWorker.h"

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
    #include <sys/types.h>
    #include <assert.h>
}

struct bluetooth_data
{
	pipe_consumer_t *consumer;
};

using namespace std;

BTSerialPortBinding *BTSerialPortBinding::Create(string address, int channelID)
{
	if (channelID <= 0)
		throw BluetoothException("ChannelID should be a positive int value");

	char addressBuffer[40];

	if (address.length() >= 40)
		throw BluetoothException("Address length is invalid");
	
	if (strcpy(addressBuffer, address.c_str()) != 0)
		throw BluetoothException("Address (first argument) length is invalid");

	return new BTSerialPortBinding(addressBuffer, channelID);
}

BTSerialPortBinding::BTSerialPortBinding(char address[40], int channelID) 
	: channelID(channelID) 
{
	data = new bluetooth_data();
	data->consumer = NULL;
	strcpy(this->address, address);
}

BTSerialPortBinding::~BTSerialPortBinding()
{
	delete data;
}

void BTSerialPortBinding::Connect()
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSString *addressString = [NSString stringWithCString:address encoding:NSASCIIStringEncoding];
    BluetoothWorker *worker = [BluetoothWorker getInstance];
    // create pipe to communicate with delegate
    pipe_t *pipe = pipe_new(sizeof(int), 0);

	int status;

    IOReturn result = [worker connectDevice: addressString onChannel:channelID withPipe:pipe];

    if (result == kIOReturnSuccess) {
        pipe_consumer_t *c = pipe_consumer_new(pipe);

        // save consumer side of the pipe
        data->consumer = c;
        status = 0;
    } else {
        status = 1;
    }

    pipe_free(pipe);
    [pool release];

	if (status != 0)
		throw BluetoothException("Cannot connect");
}

void BTSerialPortBinding::Close()
{
    NSString *addressString = [NSString stringWithCString:address encoding:NSASCIIStringEncoding];
    BluetoothWorker *worker = [BluetoothWorker getInstance];
    [worker disconnectFromDevice: addressString];
}

int BTSerialPortBinding::Read(char *buffer, int offset, int length)
{
    if (data->consumer == NULL)
        throw BluetoothException("connection has been closed");

	if (buffer == nullptr)
		throw BluetoothException("buffer cannot be null");

    size_t size = pipe_pop_eager(data->consumer, buffer + offset, length);

    if (size == 0) {
        pipe_consumer_free(data->consumer);
        data->consumer = NULL;
    }

    // when no data is read from rfcomm the connection has been closed.
    return size;
}

void BTSerialPortBinding::Write(const char *buffer, int length)
{
	if (buffer == nullptr)
		throw BluetoothException("buffer cannot be null");

	if (length == 0)
		return;

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    BluetoothWorker *worker = [BluetoothWorker getInstance];
    NSString *addressString = [NSString stringWithCString:address encoding:NSASCIIStringEncoding];

    if ([worker writeAsync: const_cast<char*>(buffer) length: length toDevice: addressString] != kIOReturnSuccess)
        throw BluetoothException("Write was unsuccessful");

    [pool release];
}