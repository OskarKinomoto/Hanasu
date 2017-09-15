#pragma once

#include <string>

#include "types.h"

namespace Hash {

std::string pbkdf2(StringRef pass, Salt salt);

std::string toHex(Salt salt);

Salt genSalt();
Salt salt(StringRef str);

uint64 guid();

SessionID genSession();

RegisterResponse::Code TestPassword(StringRef pass);

}
