#pragma once

#include <cassert>
#include <ostream>
#include <string>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "types.h"

#include "Exception.h"

enum class Domain : uint16 {
    INet = AF_INET,
    INet6 = AF_INET6,
};

enum class Type : int {
    Stream = SOCK_STREAM,
    Datagram = SOCK_DGRAM,
};

inline int operator &(Type a, Type b) {
    return static_cast<int>(a) & static_cast<int>(b);
}

inline Type operator |(Type a, Type b) {
    return static_cast<Type>(static_cast<int>(a) | static_cast<int>(b));
}

class Socket
{
public:
    Socket(Domain domain, Type type, int protocol = 0);
    Socket(const Socket&) = delete;
    Socket(Socket&&);
    Socket() = default;
    Socket& operator=(Socket&& s);
    ~Socket();

    void socket();

    void bind(uint16  port);
    void listen(int backlog);
    Socket accept();
    void connect(const std::string& hostname, const std::string &port);

    bool close();
    void shutdown();

    bool isValid();

    bool send(const void *data, size_t size, int flags = 0);
    void recv(void *data, int size, int flags = 0);
    bool pollin(int timeout_ms);

    void sendAck();
    void recvAck();

    std::string info();

protected:
    Domain m_domain;
    Type m_type;
    int m_protocol;
    int m_fd = -1;

    uint16 m_bind_port;

    bool isServer = true;

    union {
        sockaddr_in addr4;
        sockaddr_in6 addr6;
    };

    static std::ostream *io_err;

    Socket(Domain domain, Type type, int protocol, int fd);
    void Fd(int fd);
};
