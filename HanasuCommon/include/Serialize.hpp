#pragma once

#include "Protocol.hpp"
#include "types.h"

class Serialize {
public:
    static DataPtr Credentials(const Credentials&, Length &len);
    static DataPtr Bool(bool b, Length &len);
    static DataPtr LoginResponse(const LoginResponse&, Length &len);

    static DataPtr RegisterInfo(const RegisterInfo&, Length &len);
    static DataPtr RegisterResponse(const RegisterResponse&, Length &len);

    static DataPtr FindUserRequest(const FindUserRequest&, Length &len);

    static DataPtr UserList(const UserList&, Length &len);

    static DataPtr Friendship(const Friendship&, Length &len);
    static DataPtr FriendshipRes(const FriendshipRes&, Length &len);

    static DataPtr MsgSendReq(ConvID convID, StringRef msg, Length &len);
    static DataPtr MsgSendRes(uint8 success, Length &len);

    static DataPtr GetMsgsReq(ConvID convID, MsgID lastMsgID, Length &len);
    static DataPtr ConvList(ConvList convs, Length &len);

    static DataPtr UserIDs(const UserIDs &ids, Length &len);

    static DataPtr Messages(const Messages &msgs, Length &len);

};

class Deserialize {
public:
    Deserialize(Protocol::Msg msg);

    LoginResponse getLoginResponse();
    Credentials getCredentials();

    RegisterInfo getRegisterInfo();
    RegisterResponse getRegisterResponse();

    FindUserRequest getFindUserRequest();

    Friendship getFriendShip();
    FriendshipRes getFriendShipRes();

    UserList getUserList();
    UserIDs getUserIDs();

    GetMsg getGetMsg();
    ConvMsg getMsg();
    ConvList getConvList();
    Messages getMessages();

    Protocol::MsgType type();

private:
    Protocol::Msg msg;
};
