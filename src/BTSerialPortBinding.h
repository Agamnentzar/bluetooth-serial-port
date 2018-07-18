#pragma once

#include <string>
#include <memory>

struct bluetooth_data;

class BTSerialPortBinding
{
private:
	std::string address;
	int channelID;
	std::unique_ptr<bluetooth_data> data;
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
	bool IsDataAvailable();
	//void SetTimeouts(int readTimeout, int writeTimeout);
};
