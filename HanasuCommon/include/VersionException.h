#pragma once

#include "Exception.h"

class VersionException : public Exception
{
public:
    VersionException(const std::string& msg = "Protocol version is not supported") {
        m_msg = msg;
    }
};
