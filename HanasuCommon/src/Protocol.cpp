#include "Protocol.hpp"

#include "VersionException.h"
#include "Serialize.hpp"

#include <memory>
#include <iostream>

const Protocol::MsgInit Protocol::msgInitStruct{};

Protocol::Protocol(Socket &&s) : s(std::move(s))
{
}

Protocol::Protocol(Protocol &&p)
{
    std::swap(s, p.s);
    m_connection = p.m_connection;
}

Protocol &Protocol::operator=(Protocol &&p)
{
    std::swap(s, p.s);
    m_connection = p.m_connection;
    return *this;
}

void Protocol::sendInit()
{
    s.send(&msgInitStruct, sizeof(MsgInit));
}

void Protocol::recvInit()
{
    MsgInit init{};
    s.recv(&init, sizeof(init));

    if (init.version > msgInitStruct.version)
        throw VersionException{};
}

Protocol::Msg Protocol::recvMsg()
{
    Msg msg;
    s.recv(&msg.h, sizeof(msg.h));

    if (msg.h.length) {
        msg.data = makeDataPtr(msg.h.length);

        s.recv(msg.data .get(), msg.h.length);
    }

    return msg;
}

Deserialize Protocol::r()
{
    return {recvMsg()};
}

Protocol::Msg Protocol::MsgLoginReq(std::string user, std::string pass)
{
    Credentials creds{user, pass};
    Msg msg;
    msg.h.type = MsgType::LoginReq;
    msg.data = Serialize::Credentials(creds, msg.h.length);
    return msg;
}

Protocol::Msg Protocol::MsgLoginRes(UserID userID, SessionID session)
{
    LoginResponse resp{session, userID};
    Msg msg;
    msg.h.type = MsgType::LoginRes;
    msg.data = Serialize::LoginResponse(resp, msg.h.length);
    return msg;
}

Protocol::Msg Protocol::MsgRegisterRequest(std::string user, std::string pass)
{
    RegisterInfo regInfo{user, pass};
    Msg msg;
    msg.h.type = MsgType::RegisterReq;
    msg.data = Serialize::RegisterInfo(regInfo, msg.h.length);
    return msg;
}

Protocol::Msg Protocol::MsgRegisterResponse(RegisterResponse::Code code)
{
    RegisterResponse response{code};
    Msg msg;
    msg.h.type = MsgType::RegisterRes;
    msg.data = Serialize::RegisterResponse(response, msg.h.length);
    return msg;
}

Protocol::Msg Protocol::MsgFindUserRequest(StringRef user, FindUserRequest::Type type)
{
    FindUserRequest s{type, user};
    Msg msg;
    msg.h.type = MsgType::FindUserReq;
    msg.data = Serialize::FindUserRequest(s, msg.h.length);
    return msg;
}

Protocol::Msg Protocol::MsgFindUserResponse(const UserList &list)
{
    Msg msg;
    msg.h.type = MsgType::FindUserRes;
    msg.data = Serialize::UserList(list, msg.h.length);
    return msg;
}

Protocol::Msg Protocol::MsgFriendshipRequest(UserID userID, Friendship::Action action)
{
    Friendship friendship{userID, action};
    Msg msg;
    msg.h.type = MsgType::FriendshipReq;
    msg.data = Serialize::Friendship(friendship, msg.h.length);
    return msg;
}

Protocol::Msg Protocol::MsgFriendshipResponse(FriendshipRes::Code code)
{
    FriendshipRes fr{code};
    Msg msg;
    msg.h.type = MsgType::FriendshipReq;
    msg.data = Serialize::FriendshipRes(fr, msg.h.length);
    return msg;
}

Protocol::Msg Protocol::MsgLogoutRequest()
{
    Msg msg;
    msg.h.type = MsgType::LogoutReq;
    msg.h.length = 0;
    return msg;
}

Protocol::Msg Protocol::MsgLogoutResponse()
{
    Msg msg;
    msg.h.type = MsgType::LogoutRes;
    msg.h.length = 0;
    return msg;
}

Protocol::Msg Protocol::MsgFriendReqsReq()
{
    Msg msg;
    msg.h.type = MsgType::FriendReqReq;
    msg.h.length = 0;
    return msg;
}

Protocol::Msg Protocol::MsgFriendReqsRes(const UserList &ul)
{

    Msg msg;
    msg.h.type = MsgType::FriendReqRes;
    msg.data = Serialize::UserList(ul, msg.h.length);
    return msg;
}

Protocol::Msg Protocol::MsgSendMsgReq(ConvID convID, StringRef msgstr)
{\
    Msg msg;
    msg.h.type = MsgType::MsgSendReq;
    msg.data = Serialize::MsgSendReq(convID, msgstr, msg.h.length);
    return msg;
}

Protocol::Msg Protocol::MsgSendMsgRes(bool success)
{
    Msg msg;
    msg.h.type = MsgType::MsgSendRes;
    msg.data = Serialize::Bool(success, msg.h.length);
    return msg;
}

Protocol::Msg Protocol::MsgGetMsgs(ConvID convID, MsgID lastMsgID)
{
    Msg msg;
    msg.h.type = MsgType::GetMsg;
    msg.data = Serialize::GetMsgsReq(convID, lastMsgID, msg.h.length);
    return msg;
}

Protocol::Msg Protocol::MsgGetMsgsRes(Messages msgs)
{
    Msg msg;
    msg.h.type = MsgType::GetMsgRes;
    msg.data = Serialize::Messages(msgs, msg.h.length);
    return msg;
}

Protocol::Msg Protocol::MsgGetConversationsRes(const ConvList &convs)
{
    Msg msg;
    msg.h.type = MsgType::GetUsersReq;
    msg.data = Serialize::ConvList(convs, msg.h.length);
    return msg;
}

Protocol::Msg Protocol::MsgGetConversationsReq()
{
    Msg msg;
    msg.h.type = MsgType::GetConversations;
    msg.h.length = 0;
    return msg;
}

Protocol::Msg Protocol::GetUsersReq(const UserIDs &ids)
{
    Msg msg;
    msg.h.type = MsgType::GetUsersReq;
    msg.data = Serialize::UserIDs(ids, msg.h.length);
    return msg;
}

Protocol::Msg Protocol::GetUsersRes(const UserList &users)
{
    Msg msg;
    msg.h.type = MsgType::GetUsersRes;
    msg.data = Serialize::UserList(users, msg.h.length);
    return msg;
}

Protocol::Msg Protocol::MsgError()
{
    Msg msg;
    msg.h.type = MsgType::ProtocolError;
    msg.h.pc = ProtoCode::ProtocolError;
    msg.h.length = 0;
    return msg;
}

void Protocol::sendMsg(const Protocol::Msg &msg)
{
    s.send(&msg.h, sizeof(msg.h));
    s.send(msg.data.get(), msg.h.length);
}

Protocol::Msg Protocol::sendRecvMsg(const Protocol::Msg &msg)
{
    sendMsg(msg);
    return recvMsg();
}

Deserialize Protocol::sr(const Protocol::Msg &msg)
{
    sendMsg(msg);
    return Deserialize{recvMsg()};
}

bool Protocol::pollIn(int ms)
{
    return s.pollin(ms);
}

void Protocol::close()
{
    s.close();
    m_connection = false;
}

void Protocol::shutdown()
{
    s.shutdown();
}
