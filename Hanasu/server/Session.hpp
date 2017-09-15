#pragma once

#include <mutex>
#include <map>
#include <atomic>

#include <Socket.h>
#include <Protocol.hpp>

#include "DB.hpp"

class Session {
public:
    Session(Socket &&soc, DbPtr db);
    ~Session();

    void run();
    void stop();

public:
    static void stopAll();

private:
    void error();
    void login(const Credentials& creds);
    void reg(const RegisterInfo& regInfo);
    void findUser(const FindUserRequest& fur);
    void friendship(const Friendship& f);
    void users(const UserIDs& ids);
    void logout();
    void msg(ConvMsg m);
    void conversations();
    void friendReqs();
    void getMsgs(GetMsg gmsg);

private:
    Protocol c;
    DbPtr db;

    SessionID session = 0;
    UserID user = 0;

    std::atomic<bool> running{true};

private:
    typedef std::vector<Session*> Sessions;
    static std::mutex mut;

    static std::map<UserID, uint32> active_users;

    static Sessions sessions;

    const static int poll_ms = 100;
};
