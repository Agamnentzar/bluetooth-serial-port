#pragma once

#include "bt_export.h"

class BLUETOOTH_DLL Error
{
public:

    Error(int code, std::string const& message, std::string const& method);

    /// Copy constructor.
    Error(Error const& other);

    /// Assignment operator.
    Error& operator=(Error const& rhs);

    int GetCode() const { return m_code; }
    const char* GetMessage() const { return m_message.c_str(); }
    const char* GetMethod() const { return m_method.c_str(); }

private:

    int m_code;
    std::string m_message;
    std::string m_method;
};