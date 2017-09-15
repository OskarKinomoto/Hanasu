#pragma once

#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>



template< typename T >
struct array_deleter
{
    void operator ()( T const * p)
    {
        delete[] p;
    }
};

typedef u_int8_t uint8;
typedef u_int16_t uint16;
typedef u_int32_t uint32;
typedef u_int64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;

typedef unsigned int uint;

typedef std::string String;
typedef const String& StringRef;

typedef std::vector<unsigned char> bytes;
typedef const bytes& bytesRef;

typedef size_t size;

typedef uint32 Length;

typedef uint32 UserID;
typedef uint32 SessionID;
typedef uint64 ConvID;
typedef uint64 MsgID;

enum class ProtoCode : uint16 {
    OK = 0x00,
    ProtocolError = 0x01,
    OperationNotAllowed = 0x02,
    UnknownError = 0xFFFF,
};

typedef std::vector<UserID> UserIDs;

/* MSG structs */

// Login
struct Credentials {
    String user;
    String pass;
};

struct LoginResponse {
    SessionID session;
    UserID clienID;
};

struct GetMsg {
    ConvID convID;
    MsgID msgID;
};

struct ConvMsg {
    ConvID convID;
    String msg;
};



//Register
struct RegisterInfo {
    RegisterInfo() = default;
    RegisterInfo(StringRef user, StringRef pass) : nick(user), pass(pass) {}

    String nick;
    String &user = nick;
    String pass;
    String userName;
};

struct RegisterResponse {
    enum class Code : uint8 {
        None = 0xFF,
        Success = 0x00,
        UserNameExists = 0x01,
        PasswordToShort = 0x02,
        UserNameTooLong = 0x03,
    };

    Code code;
};

// Find users
struct FindUserRequest {
    enum class Type : uint8 {
        Exact = 0x00, ///< '?'
        RightSide = 0x01, ///< '?%'
        LeftSide = 0x02, ///< '%?'
        Both = RightSide | LeftSide, ///< '%?%'
    };

    Type type;
    String name;
};

struct User {
    UserID ID;
    std::string name;
};

typedef std::vector<User> UserList;

struct Participant {
    UserID ID;
};

typedef std::vector<Participant> ParticipantList;

struct Conv {
    ConvID ID;
    uint8 isPublic;
    ParticipantList participants;
    std::string name;
};

typedef std::vector<Conv> ConvList;

struct Message {
    MsgID msgID;
    ConvID convID;
    UserID userID;
    uint32 time;
    std::string message;
};

typedef std::vector<Message> Messages;

struct Friendship {
    enum class Action : uint8 {
        Request = 0x00,
        Accept = 0x01,
        Decline = 0x02,
        Remove = 0x03,
    };

    UserID userId;
    Action acition;
};

struct FriendshipRes {
    enum class Code : uint8 {
        RequestSend = 0x00,
        MultipleRequest = 0x01,
        RequestSendAndAccepted = 0x02,
        Accepted = 0x03,
        Declined = 0x04,
        FriendRemoved = 0x05,
        NoFriendToRemove = 0x06,
        Error = 0x07,
        AreFriends = 0x08,
        RequestNotExists = 0x09,
        UserDontExists = 0x0A,
    };

    Code code;
};

typedef std::vector<Message> MessageList;

struct Conversation {
    ConvID convID;
    bool pub;
    std::vector<UserID> participants;
};

struct Salt {
    uint8 s[16];
    uint8 &operator[](size_t i) { return s[i]; }
    uint8 *operator&() { return s; }
};

typedef std::shared_ptr<uint8> DataPtr;

inline DataPtr makeDataPtr(uint32 len) {
    return DataPtr(new uint8[len], array_deleter<uint8>());
}
