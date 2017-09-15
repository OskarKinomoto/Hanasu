#include "../common/include/Log.h"
#include "../common/include/Socket.h"
#include "../common/include/Protocol.hpp"
#include "../common/include/Serialize.hpp"

#include <signal.h>

UserID findUser(Protocol &s, String name) {
    UserID u = 0;
    auto list = Deserialize(s.sendRecvMsg(Protocol::MsgFindUserRequest(name,
                                                                       FindUserRequest::Type::Exact))).getUserList();
    for (auto& user : list) {
        u = user.ID;
    }
    return u;
}

using namespace std;

void populate(Protocol &s) {
    s.sendRecvMsg(Protocol::MsgRegisterRequest("user1", "pass111"));
    s.sendRecvMsg(Protocol::MsgRegisterRequest("user3", "pass333"));
    s.sendRecvMsg(Protocol::MsgRegisterRequest("user2", "pass222"));
    s.sendRecvMsg(Protocol::MsgRegisterRequest("user4", "pass444"));

    s.sendRecvMsg(Protocol::MsgLoginReq("user4", "pass444"));
    s.sendRecvMsg(Protocol::MsgFriendshipRequest(findUser(s, "user2"), Friendship::Action::Request));
    s.sendRecvMsg(Protocol::MsgFriendshipRequest(findUser(s, "user3"), Friendship::Action::Request));
    s.sendRecvMsg(Protocol::MsgLogoutRequest());

    s.sendRecvMsg(Protocol::MsgLoginReq("user2", "pass222"));
    s.sendRecvMsg(Protocol::MsgFriendshipRequest(findUser(s, "user4"), Friendship::Action::Accept));
    s.sendRecvMsg(Protocol::MsgFriendshipRequest(findUser(s, "user1"), Friendship::Action::Request));
    s.sendRecvMsg(Protocol::MsgFriendshipRequest(findUser(s, "user2"), Friendship::Action::Request));
    s.sendRecvMsg(Protocol::MsgFriendshipRequest(findUser(s, "user3"), Friendship::Action::Request));
    s.sendRecvMsg(Protocol::MsgLogoutRequest());

    s.sendRecvMsg(Protocol::MsgLoginReq("user3", "pass333"));

    auto requests = Deserialize(s.sendRecvMsg(Protocol::MsgFriendReqsReq())).getUserList();

    cout << "Requests: " << endl;

    for (auto u : requests) {
        cout << u.name << endl;
    }

    s.sendRecvMsg(Protocol::MsgFriendshipRequest(findUser(s, "user4"), Friendship::Action::Accept));

    auto convs = s.sr(Protocol::MsgGetConversationsReq()).getConvList();

    ConvID cc = 0;
    std::cout << "Ccs: "  << convs.size() << std::endl;
    for (auto c: convs) {
        cc = c.ID;
    }

    s.sendRecvMsg(Protocol::MsgSendMsgReq(cc, "qwer"));
    s.sendRecvMsg(Protocol::MsgSendMsgReq(cc, "qwer234"));
    s.sendRecvMsg(Protocol::MsgSendMsgReq(cc, "qwer434"));
    s.sendRecvMsg(Protocol::MsgSendMsgReq(cc, "qwer564"));
    s.sendRecvMsg(Protocol::MsgLogoutRequest());



    MsgID last = 0;


    s.sendRecvMsg(Protocol::MsgLoginReq("user4", "pass444"));
    auto msgs = s.sr(Protocol::MsgGetMsgs(cc, last)).getMessages();

    cout << msgs.size() << endl;
    for (auto &m : msgs) {
        cout << m.userID << " : " << m.message << endl;
        last = m.msgID > last ? m.msgID : last;
    }

    s.sendRecvMsg(Protocol::MsgLogoutRequest());

    s.sendRecvMsg(Protocol::MsgLoginReq("user3", "pass333"));
    s.sendRecvMsg(Protocol::MsgSendMsgReq(cc, "nowy!!"));
    s.sendRecvMsg(Protocol::MsgLogoutRequest());




    s.sendRecvMsg(Protocol::MsgLoginReq("user4", "pass444"));
    msgs = s.sr(Protocol::MsgGetMsgs(cc, last)).getMessages();

    cout << "Nowe: " << msgs.size() << endl;
    for (auto &m : msgs) {
        cout << m.userID << " : " << m.message << endl;
        last = m.msgID;
    }
    s.sendRecvMsg(Protocol::MsgSendMsgReq(cc, "testtt"));

    s.sendRecvMsg(Protocol::MsgLogoutRequest());
}

int main(int argc, char **) {
    try {
        Socket soc(Domain::INet6, Type::Stream);
        soc.connect("localhost", "8080");

        Protocol server(std::move(soc));
        server.recvInit();

        if (argc > 1) {
            populate(server);
            return 0;
        }
    } catch (std::exception& e) {
        LOGE(e.what());
    }

    return 0;
}
