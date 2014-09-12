#ifndef BLUETOOTH_EXCEPTION_H
#define BLUETOOTH_EXCEPTION_H

#ifndef _MSC_VER
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

#include <exception>

class BluetoothException : std::exception
{
private:
	const char* message;

public:
	BluetoothException(const char* message) NOEXCEPT
	{
		this->message = message;
	}

	virtual const char* what() const NOEXCEPT
	{
		return message;
	}
};

#endif