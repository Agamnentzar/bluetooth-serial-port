#ifndef BLUETOOTH_SERIAL_PORT_BINDING_H
#define BLUETOOTH_SERIAL_PORT_BINDING_H

#include <string>

struct bluetooth_data;

class BTSerialPortBinding
{
private:
	bluetooth_data *data;
	char address[40];
	int channelID;

	BTSerialPortBinding(char address[40], int channelID);
	~BTSerialPortBinding();

public:
	static BTSerialPortBinding *Create(std::string address, int channelID);
	void Connect();
	void Close();
	int Read(char *buffer, int offset, int length);
	void Write(char *buffer, int offset, int length);
};

#endif
