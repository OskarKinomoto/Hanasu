#include "Session.hpp"

#include <Serialize.hpp>
#include <hash.hpp>

#include <iostream>

std::mutex Session::mut{};
Session::Sessions Session::sessions{};

Session::Session(Socket &&soc, DbPtr db): c(std::move(soc)), db(db)
{
    std::lock_guard<std::mutex> lock(mut);
    sessions.push_back(this);
}

Session::~Session()
{
    stop();
}

void Session::run()
{
    c.sendInit();

    while (running) {
        if (c.pollIn(poll_ms)) {
            auto des = c.r();

            switch (des.type()) {
            case Protocol::MsgType::LoginReq:
                login(des.getCredentials());
                break;

            case Protocol::MsgType::RegisterReq:
                reg(des.getRegisterInfo());
                break;

            case Protocol::MsgType::FindUserReq:
                findUser(des.getFindUserRequest());
                break;

            case Protocol::MsgType::FriendshipReq:
                friendship(des.getFriendShip());
                break;

            case Protocol::MsgType::LogoutReq:
                logout();
                break;

            case Protocol::MsgType::GetConversations:
                conversations();
                break;

            case Protocol::MsgType::GetUsersReq:
                users(des.getUserIDs());
                break;

            case Protocol::MsgType::MsgSendReq:
                this->msg(des.getMsg());
                break;

            case Protocol::MsgType::FriendReqReq:
                friendReqs();
                break;

            case Protocol::MsgType::GetMsg:
                getMsgs(des.getGetMsg());
                break;

            default:
                error();
            }
        }
    }

}

void Session::stop()
{
    c.shutdown();
    c.close();
    running = false;
}

void Session::stopAll()
{
    std::lock_guard<std::mutex> lock(mut);

    for(auto ptr : sessions)
        ptr->running = false;
}

void Session::error()
{
    std::cout << "Not implemented!!! " << std::endl;
    auto msg = Protocol::MsgError();
    c.sendMsg(msg);
}

void Session::login(const Credentials &creds)
{

    bool was_loggd = session;
    if (!session) {
        user = db->TestCreditionals(creds);

        if (user)
            session = Hash::genSession();
    }

    auto msg = Protocol::MsgLoginRes(user, session);
    msg.h.pc = !was_loggd ? ProtoCode::OK : ProtoCode::ProtocolError;
    c.sendMsg(msg);
}

void Session::reg(const RegisterInfo &req)
{

    auto code = [&](){
        if (session)
            return RegisterResponse::Code::None;

        if (DB::TestUserNameLen(req.user))
            return RegisterResponse::Code::UserNameTooLong;

        if (db->UserExists(req.user))
            return RegisterResponse::Code::UserNameExists;

        auto res = Hash::TestPassword(req.pass);
        if (res != RegisterResponse::Code::None)
            return res;

        db->AddUser(req.user, req.pass);

        return RegisterResponse::Code::Success;

    }();

    auto msg = Protocol::MsgRegisterResponse(code);
    msg.h.pc = !session ? ProtoCode::OK : ProtoCode::ProtocolError;
    c.sendMsg(msg);
}

void Session::findUser(const FindUserRequest &fur)
{
    auto list = db->FindUsers(fur);
    auto msg = Protocol::MsgFindUserResponse(list);
    msg.h.pc = session ? ProtoCode::OK : ProtoCode::ProtocolError;
    c.sendMsg(msg);
}

void Session::friendship(const Friendship &f)
{
    auto code = [&]() {
        if (!session)
            return FriendshipRes::Code::Error;

        switch (f.acition) {

        case Friendship::Action::Request:
            if (!db->UserExists(f.userId))
                return FriendshipRes::Code::UserDontExists;
            if (db->AreFriends(user, f.userId))
                return FriendshipRes::Code::AreFriends;
            if (db->RequestIsPending(user, f.userId))
                return FriendshipRes::Code::MultipleRequest;
            if(db->SendRequest(user, f.userId))
                return FriendshipRes::Code::RequestSendAndAccepted;
            return FriendshipRes::Code::RequestSend;

        case Friendship::Action::Accept:
            if (db->AreFriends(user, f.userId))
                return FriendshipRes::Code::AreFriends;
            if (!db->RequestIsPending(f.userId, user))
                return FriendshipRes::Code::RequestNotExists;
            if (db->AcceptFriend(user, f.userId))
                return FriendshipRes::Code::Accepted;
            break;

        case Friendship::Action::Decline:
            if (!db->RequestIsPending(f.userId, user))
                return FriendshipRes::Code::RequestNotExists;
            if (db->DeclineFriend(user, f.userId))
                return FriendshipRes::Code::Declined;
            break;

        case Friendship::Action::Remove:
            if (!db->AreFriends(user, f.userId))
                return FriendshipRes::Code::NoFriendToRemove;
            if (db->RemoveFriend(user, f.userId))
                return FriendshipRes::Code::FriendRemoved;

        }

        return FriendshipRes::Code::Error;
    }();

    auto msg = Protocol::MsgFriendshipResponse(code);
    msg.h.pc = session ? ProtoCode::OK : ProtoCode::ProtocolError;
    c.sendMsg(msg);
}

void Session::users(const UserIDs &ids)
{
    c.sendMsg(Protocol::GetUsersRes(db->GetUsers(ids)));
}

void Session::logout()
{
    session = 0;
    user = 0;
    c.sendMsg(Protocol::MsgLogoutResponse());
}

void Session::msg(ConvMsg m)
{
    c.sendMsg(Protocol::MsgSendMsgRes( db->AddMsg(user, m.convID, m.msg)));
}

void Session::conversations()
{
    c.sendMsg(Protocol::MsgGetConversationsRes(db->GetUserConversation(user)));
}

void Session::friendReqs()
{
    c.sendMsg(Protocol::MsgFriendReqsRes(db->GetFriendRequests(user)));
}

void Session::getMsgs(GetMsg gmsg)
{
    if (gmsg.msgID == 0) {
        c.sendMsg(Protocol::MsgGetMsgsRes(db->GetMessages(gmsg.convID, 0)));
    } else {
        c.sendMsg(Protocol::MsgGetMsgsRes(db->GetMessagesAfter(gmsg.convID, gmsg.msgID)));
    }
}
