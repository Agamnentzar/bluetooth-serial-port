#pragma once

#ifndef _MSC_VER
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

#include <exception>
#include <string>

class BluetoothException : std::exception
{
private:
	std::string m_message;

public:
	BluetoothException(std::string message) NOEXCEPT
	{
		m_message = message;
	}

	virtual const char* what() const NOEXCEPT
	{
		return m_message.c_str();
	}
};
