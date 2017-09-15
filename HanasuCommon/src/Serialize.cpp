#include "Serialize.hpp"

#include <cstring>
#include <type_traits>

#include <iostream>

#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
#define ntohll(x) ((1==ntohl(1)) ? (x) : ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))

namespace {

// -------- LEN -------- //

template<typename T>
inline uint16 Len(T&) {
    return sizeof(T);
}

template<>
inline uint16 Len(String &str) {
    return sizeof(uint16) + str.size();
}

inline uint16 Len(String str) {
    return sizeof(uint16) + str.size();
}

template<>
inline uint16 Len(User &u) {
    return Len(u.ID) + Len(u.name);
}

template<>
inline uint16 Len(Conv &c) {
    return Len(c.ID) + Len(c.isPublic) + Len(c.name) + Len(c.participants);
}

template<>
inline uint16 Len(Message &m) {
    return Len(m.msgID) + Len(m.convID) + Len(m.userID) + Len(m.time) + Len(m.message);
}

template<typename T>
inline uint16 Len(std::vector<T> v) {
    uint16 len = sizeof(uint16);
    for (auto &itr : v)
        len += Len(itr);
    return len;
}

// -------- SER -------- //

template<typename T>
inline void ser(uint8*& ptr, T en) {
    ser(ptr, static_cast<typename std::underlying_type<T>::type>(en));
}

template<>
inline void ser(uint8*&ptr, uint8 host) {
    auto net = host;
    reinterpret_cast<decltype (net)*>(ptr)[0] = net;
    ptr += sizeof(net);
}

template<>
inline void ser(uint8*&ptr, uint16 host) {
    auto net = htons(host);
    reinterpret_cast<decltype (net)*>(ptr)[0] = net;
    ptr += sizeof(net);
}

template<>
inline void ser(uint8*&ptr, uint32 host) {
    auto net = htonl(host);
    reinterpret_cast<decltype (net)*>(ptr)[0] = net;
    ptr += sizeof(net);
}

template<>
inline void ser(uint8*&ptr, uint64 host) {
    auto net = htonll(host);
    reinterpret_cast<decltype (net)*>(ptr)[0] = net;
    ptr += sizeof(net);
}

template<>
inline void ser(uint8*& ptr, String str) {
    uint16 len = str.length();
    reinterpret_cast<decltype (len)*>(ptr)[0] = len;
    ptr += sizeof(len);
    memcpy(ptr, str.data(), str.length());
    ptr += str.length();
}

template<>
inline void ser(uint8*& ptr, User u) {
    ser(ptr, u.ID);
    ser(ptr, u.name);
}

template<typename T>
inline void ser(uint8*& ptr, std::vector<T> v) {
    uint16 len = v.size();
    ser(ptr, len);
    for (auto &itr : v)
        ser(ptr, itr);
}

// ---------- DES ---------- //

template<typename T>
inline T des(uint8*& ptr) {
    return static_cast<T>(des<typename std::underlying_type<T>::type>(ptr));
}

template<>
inline String des(uint8*& ptr) {
    uint16 len;
    len = reinterpret_cast<decltype (len)*>(ptr)[0];
    ptr += sizeof(len);
    std::string str{reinterpret_cast<char*>(ptr), len};
    ptr += str.length();
    return str;
}

template<>
inline uint8 des(uint8*& ptr) {
    uint8 ret = reinterpret_cast<decltype (ret)*>(ptr)[0];
    ptr += sizeof(ret);
    return ret;
}

template<>
inline uint16 des(uint8*& ptr) {
    uint16 ret = ntohs(reinterpret_cast<decltype (ret)*>(ptr)[0]);
    ptr += sizeof(ret);
    return ret;
}

template<>
inline uint32 des(uint8*& ptr) {
    uint32 ret = ntohl(reinterpret_cast<decltype (ret)*>(ptr)[0]);
    ptr += sizeof(ret);
    return ret;
}

template<>
inline uint64 des(uint8*& ptr) {
    uint64 ret = ntohll(reinterpret_cast<decltype (ret)*>(ptr)[0]);
    ptr += sizeof(ret);
    return ret;
}

template<typename T>
inline void des(uint8*& ptr, T &t) {
    t = des<T>(ptr);
}

} /* namespace */

DataPtr Serialize::Credentials(const ::Credentials &c, Length &len)
{
    len = Len(c.user) + Len(c.pass);
    auto data = makeDataPtr(len);
    auto ptr = data.get();
    ser(ptr, c.user);
    ser(ptr, c.pass);
    return data;
}

DataPtr Serialize::Bool(bool b, Length &len)
{
    uint8 buint = b;
    len = Len(buint);
    auto data = makeDataPtr(len);
    auto ptr = data.get();
    ser(ptr, buint);
    return data;
}

DataPtr Serialize::LoginResponse(const ::LoginResponse &res, Length &len)
{
    len = sizeof(::LoginResponse);
    auto data = makeDataPtr(len);
    auto ptr = data.get();
    ser(ptr, res.session);
    ser(ptr, res.clienID);
    return data;
}

DataPtr Serialize::RegisterInfo(const ::RegisterInfo &c, Length &len)
{
    len = Len(c.user) + Len(c.pass);
    auto data = makeDataPtr(len);
    auto ptr = data.get();
    ser(ptr, c.user);
    ser(ptr, c.pass);
    return data;
}

DataPtr Serialize::RegisterResponse(const ::RegisterResponse &res, Length &len)
{
    len = Len(res);
    auto data = makeDataPtr(len);
    auto ptr = data.get();
    ser(ptr, res.code);
    return data;
}

DataPtr Serialize::FindUserRequest(const ::FindUserRequest &req, Length &len)
{
    len = Len(req.type) + Len(req.name);
    auto data = makeDataPtr(len);
    auto ptr = data.get();
    ser(ptr, req.type);
    ser(ptr, req.name);
    return data;
}

DataPtr Serialize::UserList(const ::UserList &list, Length &len)
{
    len = Len(list);

    auto data = makeDataPtr(len);
    auto ptr = data.get();

    uint16 vecLen = list.size();
    ser(ptr, vecLen);

    for (auto &user : list) {
        ser(ptr, user.ID);
        ser(ptr, user.name);
    }

    return data;
}

DataPtr Serialize::Friendship(const ::Friendship &f, Length &len)
{
    len = sizeof(::Friendship);
    auto data = makeDataPtr(len);
    auto ptr = data.get();
    ser(ptr, f.userId);
    ser(ptr, f.acition);
    return data;
}

DataPtr Serialize::FriendshipRes(const ::FriendshipRes &fr, Length &len)
{
    len = Len(fr);
    auto data = makeDataPtr(len);
    auto ptr = data.get();
    ser(ptr, fr.code);
    return data;
}

DataPtr Serialize::MsgSendReq(ConvID convID, StringRef msg, Length &len)
{
    len = sizeof(convID) + Len(msg);
    auto data = makeDataPtr(len);
    auto ptr = data.get();
    ser(ptr, convID);
    ser(ptr, msg);
    return data;
}

DataPtr Serialize::MsgSendRes(uint8 success, Length &len)
{
    len = Len(success);
    auto data = makeDataPtr(len);
    auto ptr = data.get();
    ser(ptr, success);
    return data;
}

DataPtr Serialize::GetMsgsReq(ConvID convID, MsgID lastMsgID, Length &len)
{
    len = Len(convID) + Len(lastMsgID);
    auto data = makeDataPtr(len);
    auto ptr = data.get();
    ser(ptr, convID);
    ser(ptr, lastMsgID);
    return data;
}

DataPtr Serialize::ConvList(::ConvList convs, Length &len)
{
    len = Len(convs);

    auto data = makeDataPtr(len);
    auto ptr = data.get();

    uint16 vecLen = convs.size();
    ser(ptr, vecLen);

    for (auto &conv : convs) {
        ser(ptr, conv.ID);
        ser(ptr, conv.isPublic);

        vecLen = conv.participants.size();
        ser(ptr, vecLen);
        for (auto &par : conv.participants) {
            ser(ptr, par.ID);
        }

        ser(ptr, conv.name);
    }

    return data;
}

DataPtr Serialize::UserIDs(const ::UserIDs &ids, Length &len)
{
    len = Len(ids);

    auto data = makeDataPtr(len);
    auto ptr = data.get();

    uint16 vecLen = ids.size();
    ser(ptr, vecLen);

    for (auto &id : ids) {
        ser(ptr, id);
    }

    return data;
}

DataPtr Serialize::Messages(const ::Messages &msgs, Length &len)
{
    len = Len(msgs);

    auto data = makeDataPtr(len);
    auto ptr = data.get();

    uint16 vecLen = msgs.size();
    ser(ptr, vecLen);

    for (auto &m : msgs) {
        ser(ptr, m.msgID);
        ser(ptr, m.convID);
        ser(ptr, m.userID);
        ser(ptr, m.time);
        ser(ptr, m.message);
    }

    return data;
}

Deserialize::Deserialize(Protocol::Msg msg) : msg(msg) {}

LoginResponse Deserialize::getLoginResponse()
{
    LoginResponse ret;
    uint8 *ptr = msg.data.get();
    des(ptr, ret.session);
    des(ptr, ret.clienID);
    return ret;
}

Credentials Deserialize::getCredentials()
{
    Credentials creds;
    uint8 *ptr = msg.data.get();
    des(ptr, creds.user);
    des(ptr, creds.pass);
    return creds;
}

RegisterInfo Deserialize::getRegisterInfo()
{
    RegisterInfo info;
    uint8 *ptr = msg.data.get();
    des(ptr, info.user);
    des(ptr, info.pass);
    return info;
}

RegisterResponse Deserialize::getRegisterResponse()
{
    RegisterResponse ret;
    uint8 *ptr = msg.data.get();
    des(ptr, ret.code);
    return ret;
}

FindUserRequest Deserialize::getFindUserRequest()
{
    FindUserRequest ret;
    uint8 *ptr = msg.data.get();
    des(ptr, ret.type);
    des(ptr, ret.name);
    return ret;
}

Friendship Deserialize::getFriendShip()
{
    Friendship ret;
    uint8 *ptr = msg.data.get();
    des(ptr, ret.userId);
    des(ptr, ret.acition);
    return ret;
}

FriendshipRes Deserialize::getFriendShipRes()
{
    FriendshipRes ret;
    uint8 *ptr = msg.data.get();
    des(ptr, ret.code);
    return ret;
}

UserList Deserialize::getUserList()
{
    UserList ret;
    uint8 *ptr = msg.data.get();
    uint16 vecLen = 0;
    des(ptr, vecLen);

    ret.reserve(vecLen);

    for (decltype(vecLen) i = 0; i < vecLen; ++i) {
        User u;
        des(ptr, u.ID);
        des(ptr, u.name);
        ret.push_back(u);
    }

    return ret;
}

UserIDs Deserialize::getUserIDs()
{
    UserIDs ret;
    uint8 *ptr = msg.data.get();
    uint16 vecLen = 0;
    des(ptr, vecLen);

    ret.reserve(vecLen);

    for (decltype(vecLen) i = 0; i < vecLen; ++i) {
        UserID u;
        des(ptr, u);
        ret.push_back(u);
    }

    return ret;
}

GetMsg Deserialize::getGetMsg()
{
    GetMsg msg;
    uint8 *ptr = this->msg.data.get();
    des(ptr, msg.convID);
    des(ptr, msg.msgID);
    return msg;
}

ConvMsg Deserialize::getMsg()
{
    ConvMsg msg{0,""};
    uint8 *ptr = this->msg.data.get();
    des(ptr, msg.convID);
    des(ptr, msg.msg);
    return msg;
}

ConvList Deserialize::getConvList()
{
    ConvList ret{};

    uint8 *ptr = msg.data.get();
    uint16 vecLen = 0;
    des(ptr, vecLen);

    ret.reserve(vecLen);

    for (decltype(vecLen) i = 0; i < vecLen; ++i) {
        Conv c;
        des(ptr, c.ID);
        des(ptr, c.isPublic);

        uint16 vecLen2 = 0;
        des(ptr, vecLen2);
        c.participants.reserve(vecLen2);

        for (decltype(vecLen2) j = 0; j < vecLen2; ++j) {
            Participant p;
            des(ptr, p.ID);
            c.participants.push_back(p);
        }


        des(ptr, c.name);

        ret.push_back(c);
    }

    return ret;
}

Messages Deserialize::getMessages()
{
    Messages ret{};

    uint8 *ptr = msg.data.get();
    uint16 vecLen = 0;
    des(ptr, vecLen);

    ret.reserve(vecLen);

    for (decltype(vecLen) i = 0; i < vecLen; ++i) {
        Message m{};
        des(ptr, m.msgID);
        des(ptr, m.convID);
        des(ptr, m.userID);
        des(ptr, m.time);
        des(ptr, m.message);
        ret.push_back(m);
    }

    return ret;
}

Protocol::MsgType Deserialize::type()
{
    return msg.h.type;
}
