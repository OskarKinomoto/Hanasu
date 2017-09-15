#pragma once

#include <exception>
#include <string>

class Exception : public std::exception
{
public:
    Exception(const std::string& msg) : m_msg(msg) {}
    Exception(const char* msg) : m_msg(msg) {}
    virtual ~Exception() noexcept{}

    const char* what() const noexcept override{
       return m_msg.c_str();
    }

protected:
    Exception() = default;

    std::string m_msg;
};
