#include <string>
#include <stdlib.h>
#include <unistd.h>
#include "BluetoothException.h"
#include "BTSerialPortBinding.h"

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

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>
}

using namespace std;

struct bluetooth_data
{
	int s;
	int rep[2];
};

BTSerialPortBinding *BTSerialPortBinding::Create(std::string address, int channelID)
{
	if (channelID <= 0)
		throw BluetoothException("ChannelID should be a positive int value");

	char addressBuffer[40];

	if (address.length() >= 40)
		throw BluetoothException("Address length is invalid");
	
	strcpy(addressBuffer, address.c_str());

	return new BTSerialPortBinding(addressBuffer, channelID);
}

BTSerialPortBinding::BTSerialPortBinding(char address[40], int channelID)
	: channelID(channelID)
{
	data = new bluetooth_data();
	data->s = 0;
	strcpy(this->address, address);
}

BTSerialPortBinding::~BTSerialPortBinding()
{
	Close();
	delete data;
}

void BTSerialPortBinding::Connect()
{
	Close();

	// allocate an error pipe
	if (pipe(data->rep) == -1)
	{
		string err("Cannot create pipe for reading - ");
		throw BluetoothException(err + strerror(errno));
	}

	int flags = fcntl(data->rep[0], F_GETFL, 0);
	fcntl(data->rep[0], F_SETFL, flags | O_NONBLOCK);

	struct sockaddr_rc addr = {
		0x00,
		{ { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
		0x00
	};

	// allocate a socket
	data->s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	// set the connection parameters (who to connect to)
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t)channelID;
	str2ba(address, &addr.rc_bdaddr);

	// connect to server
	int status = connect(data->s, (struct sockaddr *)&addr, sizeof(addr));
	int sock_flags = fcntl(data->s, F_GETFL, 0);
	fcntl(data->s, F_SETFL, sock_flags | O_NONBLOCK);

	if (status != 0)
		throw BluetoothException("Cannot connect");
}

void BTSerialPortBinding::Close()
{
	if (data->s != 0)
	{
		close(data->s);
		write(data->rep[1], "close", (strlen("close") + 1));
		data->s = 0;
	}
}

int BTSerialPortBinding::Read(char *buffer, int length)
{
	if (data->s == 0)
		throw BluetoothException("connection has been closed");

	if (buffer == nullptr)
		throw BluetoothException("buffer cannot be null");

	if (length == 0)
		return 0;

	fd_set set;
	FD_ZERO(&set);
	FD_SET(data->s, &set);
	FD_SET(data->rep[0], &set);

	int nfds = (data->s > data->rep[0]) ? data->s : data->rep[0];
	int size = -1;

	if (pselect(nfds + 1, &set, NULL, NULL, NULL, NULL) >= 0)
	{
		if (FD_ISSET(data->s, &set))
			size = read(data->s, buffer, length);
		else // when no data is read from rfcomm the connection has been closed.
			size = 0;
	}

	if (size < 0)
		throw BluetoothException("Error reading from connection");

	return size;
}

void BTSerialPortBinding::Write(const char *buffer, int length)
{
	if (buffer == nullptr)
		throw BluetoothException("buffer cannot be null");

	if (length == 0)
		return;

	if (data->s == 0)
		throw BluetoothException("Attempting to write to a closed connection");

	if (write(data->s, buffer, length) != length)
		throw BluetoothException("Writing attempt was unsuccessful");
}