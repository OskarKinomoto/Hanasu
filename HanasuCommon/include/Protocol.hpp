#pragma once
#include <memory>
#include "types.h"

#include "Socket.h"

class Deserialize;

class Protocol {
public:
    enum class MsgType : uint16 {
        Response = 0x0001,
        ProtocolError = 0xFFFE,
        Init     = 0xFFFF,


        LoginReq = 0x0002,
        LoginRes = LoginReq | 0x0001,

        RegisterReq = 0x0004,
        RegisterRes = RegisterReq | 0x0001,

        FindUserReq = 0x0006,
        FindUserRes = FindUserReq | Response,

        FriendshipReq = 0x0008,
        FriendshipRes = FriendshipReq | Response,

        LogoutReq = 0x000A,
        LogoutRes = LogoutReq | Response,

        MsgSendReq = 0x000C,
        MsgSendRes = MsgSendReq | Response,

        GetUsersReq = 0x000E,
        GetUsersRes = GetUsersReq | Response,

        GetConversations = 0x0010,
        GetConversationsRes = GetConversations | Response,

        FriendReqReq = 0x0012,
        FriendReqRes = FriendReqReq | Response,

        GetMsg = 0x0014,
        GetMsgRes = GetMsg | Response,
    };

    struct Header {
        uint32 length;
        MsgType type;
        ProtoCode pc;
    };

    struct MsgInit {
        uint16 version = 0x02;
    };

    struct Msg {
        Header h;
        std::shared_ptr<uint8> data;
    };

    Protocol() = default;
    Protocol(Socket&& s);
    Protocol(Protocol&& p);
    Protocol& operator=(Protocol&& p);

    void sendInit();
    void recvInit();


    static Msg MsgLoginReq(std::string user, std::string pass);
    static Msg MsgLoginRes(UserID userID, SessionID session);

    static Msg MsgRegisterRequest(std::string user, std::string pass);
    static Msg MsgRegisterResponse(RegisterResponse::Code code);

    static Msg MsgFindUserRequest(StringRef user, FindUserRequest::Type type);
    static Msg MsgFindUserResponse(const UserList& list);

    static Msg MsgFriendshipRequest(UserID userID, Friendship::Action action);
    static Msg MsgFriendshipResponse(FriendshipRes::Code code);

    static Msg MsgLogoutRequest();
    static Msg MsgLogoutResponse();

    static Msg MsgFriendReqsReq();
    static Msg MsgFriendReqsRes(const UserList &ul);

    static Msg MsgSendMsgReq(ConvID convID, StringRef msg);
    static Msg MsgSendMsgRes(bool success);

    static Msg MsgGetMsgs(ConvID convID, MsgID lastMsgID);
    static Msg MsgGetMsgsRes(Messages msgs);


    static Msg MsgGetConversationsReq();
    static Msg MsgGetConversationsRes(const ConvList &conv);

    static Msg GetUsersReq(const UserIDs &ids);
    static Msg GetUsersRes(const UserList &users);

    static Msg MsgError();

    Msg recvMsg();
    Deserialize r();

    void sendMsg(const Msg &msg);
    Msg sendRecvMsg(const Msg& msg);
    Deserialize sr(const Msg& msg);

    bool pollIn(int ms);

    void close();
    void shutdown();

private:
    Socket s;
    bool m_connection = true;

private:
    static const MsgInit msgInitStruct;
};
