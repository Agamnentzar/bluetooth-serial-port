#ifndef BLUETOOTH_SERIAL_PORT_BINDING_H
#define BLUETOOTH_SERIAL_PORT_BINDING_H

#include <string>

struct bluetooth_data;

class BTSerialPortBinding
{
private:
	bluetooth_data *data;
	std::string address;
	int channelID;
	//int readTimeout;
	//int writeTimeout;
	BTSerialPortBinding(std::string address, int channelID);

public:
	~BTSerialPortBinding();
	static BTSerialPortBinding *Create(std::string address, int channelID);
	void Connect();
	void Close();
	int Read(char *buffer, int length);
	void Write(const char *buffer, int length);
	//void SetTimeouts(int readTimeout, int writeTimeout);
};

#endif
