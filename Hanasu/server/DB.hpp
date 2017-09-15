#pragma once

#include <string>
#include <vector>
#include <memory>

#include <types.h>

struct sqlite3;

class DB {
public:

public:
    DB();
    ~DB();

    void CreateUserTable();
    void CreateFriendsTable();
    void CreateConversationsTables();
    void CreateConversationTable();
    void CreateMessagesTable();
    void CreateParticipantsTable();

    bool UserExists(const std::string& name);
    bool UserExists(UserID uid);
    UserID TestCreditionals(const Credentials& creds);

    bool AddUser(const std::string& name, const std::string& pass);

    UserList FindUsers(const FindUserRequest &f);
    bool AreFriends(UserID a, UserID b);
    bool RequestIsPending(UserID user, UserID f);
    bool SendRequest(UserID user, UserID f);
    bool AcceptFriend(UserID user, UserID f);
    bool DeclineFriend(UserID user, UserID f);
    bool RemoveFriend(UserID user, UserID f);

    ConvID AddFriendsConversation(UserID a, UserID b);
    bool AddParticipant(ConvID conv, UserID user);

    ConvList GetUserConversation(UserID user);
    Conv GetConversation(ConvID convID);
    ParticipantList GetParticipants(ConvID convID);

    UserList GetUsers(UserIDs uids);

    UserList GetFriendRequests(UserID userID);

    Messages GetMessages(ConvID convID, uint32 offset);
    Messages GetMessagesAfter(ConvID convID, MsgID msgID);

    bool AddMsg(UserID userID, ConvID convID, StringRef msg);

    bool IsParticipant(ConvID convID, UserID userID);

    static bool TestUserNameLen(StringRef name);
private:
    sqlite3 *db;

private:
    void testErr(int err);
};

typedef std::shared_ptr<DB> DbPtr;
