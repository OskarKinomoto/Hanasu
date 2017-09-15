#include "hash.hpp"

#include "Exception.h"

#include <openssl/evp.h>
#include <openssl/rand.h>

namespace Hash {

namespace {

bool randInitialized = false;

inline void randInit() {
    if (!randInitialized) {
        RAND_poll();
        randInitialized = true;
    }
}

}

std::string pbkdf2(StringRef pass, Salt salt)
{
    uint8 buf[20];

    PKCS5_PBKDF2_HMAC_SHA1(pass.c_str(), pass.length(),
                           salt.s, sizeof(salt),
                           1024,
                           sizeof(buf), buf);

    char cbuf[sizeof(buf) * 2];

    for (uint i = 0; i < sizeof(buf); ++i) {
        cbuf[2*i] = (buf[i] & 0x0F) + '0';
        cbuf[2*i + 1] = (buf[i] >> 4) + '0';
    }

    return {cbuf, sizeof(cbuf)};
}

std::string toHex(Salt salt)
{
    char cbuf[sizeof(salt) * 2];

    for (uint i = 0; i < sizeof(salt); ++i) {
        cbuf[2*i] = (salt[i] >> 4) + '0';
        cbuf[2*i + 1] = (salt[i] & 0x0F) + '0';
    }

    return {cbuf, sizeof(cbuf)};
}

Salt genSalt()
{
    randInit();

    Salt s;
    RAND_bytes(&s, sizeof(s));
    return s;
}

Salt salt(StringRef str)
{
    Salt salt;

    if (str.length() != sizeof(salt) * 2)
        throw Exception("salt err");

    for (uint i = 0; i < sizeof(salt); ++i) {
        salt[i] = ((str[2*i] - '0') << 4) | (str[2*i + 1] - '0');
    }

    return salt;
}

RegisterResponse::Code TestPassword(StringRef pass)
{
    if (pass.length() < 6)
        return RegisterResponse::Code::PasswordToShort;

    return RegisterResponse::Code::None;
}

SessionID genSession()
{
    randInit();

    SessionID s;
    RAND_bytes(reinterpret_cast<uint8*>(&s), sizeof(s));
    return s;
}

uint64 guid()
{
    randInit();
    uint64 s;
    RAND_bytes(reinterpret_cast<uint8*>(&s), sizeof(s));
    return s;
}

}
