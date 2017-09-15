#include "controler.h"

#include <Log.h>
#include <Socket.h>
#include <Protocol.hpp>
#include <Serialize.hpp>
#include <ErrnoException.h>

#define MAX_SET(member, val) (member = (member >= val) ? (member) : (val))

namespace {
UserID findUser(Protocol &s, String name) {
    UserID u = 0;
    auto list = Deserialize(s.sendRecvMsg(Protocol::MsgFindUserRequest(name,
                                                                       FindUserRequest::Type::Exact))).getUserList();
    for (auto& user : list) {
        u = user.ID;
    }
    return u;
}
}

Controler::Controler()
{

}

Conversations *Controler::getConv()
{
    return &c;
}

MessagesList *Controler::getMsgList()
{
    return &mlist;
}

FriendsModel *Controler::getFModel()
{
    return &flist;
}

void Controler::init()
{
    if (m_init)
        return;

    Socket soc(Domain::INet6, Type::Stream);
    soc.connect("127.0.0.1", "8080");
    s = std::move(Protocol(std::move(soc)));
    s.recvInit();

    m_init = true;
}

void Controler::login(QString name, QString pass)
{
    try {
    init();

    auto msg = s.sendRecvMsg(Protocol::MsgLoginReq(name.toStdString(), pass.toStdString()));
    m_login = Deserialize(msg).getLoginResponse();
    std::cout << m_login.clienID << std::endl;
    if (m_login.clienID == 0)
        loginFailed();
    else
        loginSuccess();

    u = m_login.clienID;
    } catch (ErrnoException &e) {
        alert("Can not connect to server!");
        LOGE(e.what());
    }
}

void Controler::registration(QString name, QString pass)
{
    init();
    auto msg = s.sr(Protocol::MsgRegisterRequest(name.toStdString(),
                                               pass.toStdString())).getRegisterResponse();

    if (msg.code == RegisterResponse::Code::Success)
        registerSuccess();
    else
        alert("Register error!");

}

void Controler::addFriend(QString name)
{
    String str = name.toStdString();
    auto res = Deserialize(s.sendRecvMsg(Protocol::MsgFriendshipRequest(findUser(s, str), Friendship::Action::Request))).getFriendShipRes();

    switch (res.code) {
    case FriendshipRes::Code::RequestSend:
        alert("Request send!");
        break;
    case FriendshipRes::Code::UserDontExists:
        alert("User <b>" + name + "</b> does not exists!");
        break;
    case FriendshipRes::Code::MultipleRequest:
        alert("You already send friendship request to <b>" + name + "</b>!");
        break;
    default:
        alert(QString{"Unknown error "} + std::to_string((int)res.code).c_str());
    }

}

bool Controler::fetchConv()
{
    auto convList2 = s.sr(Protocol::MsgGetConversationsReq()).getConvList();

    std::swap(convList2, convList);

    if (convList.size() == convList2.size())
        return false;

    ConvID aID = c.aID;

    c.clean();


    for (auto& conv : convList) {
        UserID fID = conv.participants[0].ID == u ?
                    conv.participants[1].ID : conv.participants[0].ID;

        c.addConversation(conv.ID,
                          s.sr(Protocol::GetUsersReq({fID})).getUserList()[0].name.c_str());
    }

    c.activate(aID);

    return true;
}

void Controler::showMsgs(int convID)
{
    cID = convID;
    mlist.clear();
    mlist.uid = u;
    auto msgs = s.sr(Protocol::MsgGetMsgs(convID, 0)).getMessages();

    for (int i = msgs.size() - 1; i >= 0; --i) {
        auto& m = msgs[i];
        MAX_SET(last, m.msgID);
        mlist.Add(m.msgID, m.userID, m.message.c_str());
    }
}

void Controler::newMsgs()
{
    if (cID == 0)
        return;


    auto msgs = s.sr(Protocol::MsgGetMsgs(cID, last)).getMessages();

    for (int i = msgs.size() - 1; i >= 0; --i) {
        auto& m = msgs[i];
        MAX_SET(last, m.msgID);
        mlist.Add(m.msgID, m.userID, m.message.c_str());
    }
}

void Controler::fetchFriends()
{
    auto freq = s.sr(Protocol::MsgFriendReqsReq()).getUserList();

    flist.clean();

    for (auto& u: freq) {
        flist.Add(u.ID, u.name.c_str(), FriendsModel::FriendRequest);
    }

}

void Controler::sendMsg(QString msg)
{
    if (cID == 0)
        return;

    s.sr(Protocol::MsgSendMsgReq(cID, msg.toStdString()));

    newMsgs();
}

void Controler::acceptFriend(int id)
{
    s.sr(Protocol::MsgFriendshipRequest(id, Friendship::Action::Accept));
    fetchFriends();
}

void Controler::rejectFriend(int id)
{
    s.sr(Protocol::MsgFriendshipRequest(id, Friendship::Action::Decline));
    fetchFriends();
}
