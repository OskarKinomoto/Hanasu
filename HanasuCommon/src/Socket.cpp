#include "Socket.h"

#include <iostream>
#include <unistd.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdexcept>
#include <unistd.h>
#include <poll.h>

#include <netdb.h>

#include "AckException.h"
#include "ErrnoException.h"
#include "Log.h"

#if EAGAIN == EWOULDBLOCK
#define case_EAGAIN case EAGAIN
#else
#define case_EAGAIN case EAGAIN: case EWOULDBLOCK
#endif

namespace {

constexpr uint16 ackCode = 0xACDC;

}

std::ostream *Socket::io_err = &std::cerr;

Socket::Socket(Domain domain, Type type, int protocol) : m_domain(domain),
    m_type(type), m_protocol(protocol), m_fd(-1), m_bind_port(0)
{}

Socket::Socket(Socket &&s)
{
    m_domain = s.m_domain;
    m_type = s.m_type;
    m_protocol = s.m_protocol;
    m_fd = s.m_fd;

    s.m_fd = -1;
}

Socket &Socket::operator=(Socket &&s)
{
    m_domain = s.m_domain;
    m_type = s.m_type;
    m_protocol = s.m_protocol;
    m_fd = s.m_fd;

    s.m_fd = -1;
    return *this;
}

Socket::~Socket()
{
    close();
}

void Socket::socket()
{
    m_fd = ::socket(static_cast<int>(m_domain), static_cast<int>(m_type), m_protocol);

    if (m_fd == -1)
        throw ErrnoException("socket creation failed: ");

    LOGD("Created socket <" << info() << ">");
}

void Socket::bind(uint16 port)
{
    sockaddr *addr = nullptr;
    size_t len = 0;
    int result;

    socket();

    switch (m_domain) {
    case Domain::INet:
        addr4.sin_family = static_cast<uint16>(m_domain);
        addr4.sin_addr.s_addr = 0x00000000;
        addr4.sin_port = htons(port);
        addr = reinterpret_cast<sockaddr*>(&addr4);
        len = sizeof(addr4);
        break;
    case Domain::INet6:
        addr6.sin6_family = static_cast<uint16>(m_domain);
        addr6.sin6_addr = in6addr_any;
        addr6.sin6_port = htons(port);
        addr6.sin6_scope_id = 0;
        addr6.sin6_flowinfo = 0;
        addr = reinterpret_cast<sockaddr*>(&addr6);
        len = sizeof(addr6);
        break;
    }


    result = TEMP_FAILURE_RETRY(::bind(m_fd,addr,len));

    if (result == -1)
        throw ErrnoException("Binding socket to " + std::to_string(port) + " port failed");

    m_bind_port = port;
    LOGD("Bind socket <" << info() << ">");
}

void Socket::listen(int backlog)
{
    int result = ::listen(m_fd, backlog);
    if (result == -1)
        throw ErrnoException("Listening on socket '" + std::to_string(m_fd) +
                             "' failed");
    LOGD("Listen socket <" << info() << ">");
}

Socket Socket::accept()
{
    int result;

    sockaddr *addr = nullptr;
    size_t len = 0;

    Socket s = Socket(m_domain, m_type, m_protocol, 0);

    switch (m_domain) {
    case Domain::INet:
        addr = reinterpret_cast<sockaddr*>(&s.addr4);
        len = sizeof(s.addr4);
        break;
    case Domain::INet6:
        addr = reinterpret_cast<sockaddr*>(&s.addr6);
        len = sizeof(s.addr6);
        break;
    }

    result = TEMP_FAILURE_RETRY(::accept(m_fd, addr, reinterpret_cast<socklen_t*>(&len)));

    if (result == -1) {
        switch (errno) {
case_EAGAIN:
        break;
        default:
            throw ErrnoException("Accepting on socket '"+ std::to_string(m_fd) +
                                 "' failed");
        }
    }

    s.Fd(result);

    LOGD("Accepted socket <" << s.info() << "> on socket <" << info() << ">");
    return s;
}

void Socket::connect(const std::string &hostname, const std::string &port)
{
    addrinfo hints;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = static_cast<int>(m_type);
    hints.ai_protocol = 0;
    hints.ai_addr = nullptr;
    hints.ai_canonname = nullptr;
    hints.ai_next = nullptr;

    addrinfo *results = nullptr;

    int s = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &results);

    if (s != 0)
        throw Exception(std::string{"getaddrinfo failed: "} + gai_strerror(s));

    close();

    addrinfo *res = results;

    for(;res; res = res->ai_next) {
        m_fd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        if (m_fd == -1)
            continue;

        if (::connect(m_fd, res->ai_addr, res->ai_addrlen) == 0)
            break;

        close();
    }

    if (m_fd == -1)
        throw ErrnoException("Connecting socket '" + std::to_string(m_fd) +
                             "' to '" + hostname + ":" + port + "' failed");

    LOGD("Socket <" << info() << "> connected to " << hostname << ":" << port);

    freeaddrinfo(results);

}

bool Socket::close()
{
    int result = 0;
    if (isValid()) {
        result = TEMP_FAILURE_RETRY(::close(m_fd));
        if (result == -1) {
            int err = errno;
            LOGE("Closing socket <" << info() << "> failed: " <<
                 ErrnoException::getErrnoString(err));
        } else {
            LOGD("Closed socket <" << info() << ">");
        }

        m_fd = -1;
    }
    return result == 0;
}

void Socket::shutdown()
{
    ::shutdown(m_fd, SHUT_RDWR);
}

bool Socket::isValid()
{
    return m_fd != -1;
}

bool Socket::send(const void *data, size_t size, int flags)
{
    int result = 0;
    size_t dataSend = 0;

    while (dataSend < size) {
        result = ::send(m_fd, static_cast<const char*>(data) + dataSend,
                        size - dataSend, flags | MSG_NOSIGNAL);

        if (result == -1)
            throw ErrnoException("Sending data to socket <" +
                                 info() + "> failed");
        dataSend += result;
        LOGD("Send " << dataSend << "/" << size << " on socket <" << info() <<
             ">");
    }

    return true;

}

void Socket::recv(void *data, int size, int flags)
{
    int result;
    int dataRecieved = 0;

    while(dataRecieved < size) {
        result = ::recv(m_fd, static_cast<char*>(data) + dataRecieved,
                        size - dataRecieved, flags);

        if (result == -1)
            throw ErrnoException("Recieving data from socket <" +
                                 info() + "> failed");

        if (result == 0)
            throw ErrnoException("", EPIPE);

        dataRecieved += result;

        LOGD("Recieved " << dataRecieved << "/" << size << " on socket <" <<
             info() << ">");
    }
}

bool Socket::pollin(int timeout_ms)
{
    pollfd fds[1];
    fds->fd = m_fd;
    fds->revents = 0;
    fds->events = POLLIN;

    int rc = poll(fds, 1, timeout_ms);

    if (rc == -1 && errno == EINTR)
        return false;

    if (rc == -1)
        throw ErrnoException("Poll");

    if (rc == 0)
        return false;

    return true;
}

void Socket::sendAck()
{
    send(&ackCode, sizeof(ackCode));
    LOGD("Send ack " << ackCode << " to socket <" << info() << ">");
}

void Socket::recvAck()
{
    auto ack = decltype(ackCode)(0);
    recv(&ack, sizeof(ack));

    if (ack != ackCode) {
        LOGD("Recieved incorrect ack " << ack << "instead of " << ackCode <<
             " from socket <" << info() << ">");
        throw AckException();
    }

    LOGD("Recieved ack " << ackCode << " from socket <" << info() << ">");
}

Socket::Socket(Domain domain, Type type, int protocol, int fd) :
    m_domain(domain), m_type(type), m_protocol(protocol), m_fd(fd),
    m_bind_port(0)
{
    isServer = false;
    LOGD("Constructed socket <" << info() << ">");
}

void Socket::Fd(int fd)
{
    m_fd = fd;
}

std::string Socket::info()
{
    std::stringstream s;
#ifdef BUILD_TYPE_DEBUG
    s << "fd " << m_fd << " | ptr " << reinterpret_cast<size_t>(this)
      << " | path '" << m_bind_path << "'";
#else
    s << m_fd;
#endif
    return s.str();
}
