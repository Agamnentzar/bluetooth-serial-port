#include "capi/bt_impl.h"

Error::Error(int code, std::string const& message, std::string const& method) :
    m_code(code),
    m_message(message),
    m_method(method)
{
}

Error::Error(Error const& other) :
    m_code(other.m_code),
    m_message(other.m_message),
    m_method(other.m_method)
{
}

Error& Error::operator=(Error const& rhs)
{
    if (&rhs != this)
    {
        m_code = rhs.m_code;
        m_message = rhs.m_message;
        m_method = rhs.m_method;

    }
    return *this;
}