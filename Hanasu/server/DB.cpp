#include "DB.hpp"

#include <sqlite3.h>
#include <Exception.h>
#include <hash.hpp>
#include <Log.h>

#include <string>
#include <iostream>

namespace {

const std::string path = "gawariet.db";

inline const char* string_cast(const unsigned char* ptr) {
    return reinterpret_cast<const char*>(ptr);
}

}

DB::DB()
{
    int ret = sqlite3_open(path.c_str(), &db);

    if (ret)
        throw Exception("Can not open database: " + path);

    sqlite3_exec(db, "PRAGMA foreign_keys = ON", nullptr, nullptr, nullptr);

    CreateUserTable();
    CreateFriendsTable();
    CreateConversationsTables();
}

DB::~DB()
{
    sqlite3_close(db);
}

void DB::CreateUserTable()
{
    char *errMsg = nullptr;

    char query[] = "CREATE TABLE IF NOT EXISTS users ("
                   "ID INTEGER PRIMARY KEY NOT NULL, "
                   "name VARCHAR(300), "
                   "salt VARCHAR(32), "
                   "hash VARCHAR(40) "
                   ")";

    int rc = sqlite3_exec(db, query, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << errMsg << std::endl;
    } else {
        std::cout << "Users created!" << std::endl;
    }
}

void DB::CreateFriendsTable()
{
    char *errMsg = nullptr;
    char query[] = "CREATE TABLE IF NOT EXISTS friends ("
                   "user_a INTEGER, "
                   "user_b INTEGER, "
                   "accepted INT(1) DEFAULT 0, "
                   "convID INTEGER, "
                   "FOREIGN KEY(user_a) REFERENCES users(id), "
                   "FOREIGN KEY(user_b) REFERENCES users(id) "
                   "FOREIGN KEY(convID) REFERENCES conv(convID)"
                   ")";

    int rc = sqlite3_exec(db, query, nullptr, nullptr, &errMsg);
    testErr(rc);
    if (rc != SQLITE_OK) {
        std::cerr << "Friends: " << errMsg << std::endl;
    } else {
        std::cout << "Friends created!" << std::endl;
    }
}

void DB::CreateConversationsTables()
{
    CreateConversationTable();
    CreateMessagesTable();
    CreateParticipantsTable();
}

void DB::CreateConversationTable()
{
    char *errMsg = nullptr;
    char query[] = "CREATE TABLE IF NOT EXISTS conv ("
                   "convID INTEGER PRIMARY KEY NOT NULL, "
                   "name VARCHAR(100) DEFAULT '', "
                   "time TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
                   "guid INTEGER, "
                   "public INT(1) DEFAULT 0"
                   ")";

    int rc = sqlite3_exec(db, query, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Friends: " << errMsg << std::endl;
    } else {
        std::cout << "Friends created!" << std::endl;
    }
}

void DB::CreateMessagesTable()
{
    char *errMsg = nullptr;
    char query[] = "CREATE TABLE IF NOT EXISTS messages ("
                   "msgID INTEGER PRIMARY KEY NOT NULL, "
                   "convID INTEGER, "
                   "userID INTEGER, "
                   "time TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
                   "message TEXT, "
                   "FOREIGN KEY(convID) REFERENCES conv(convID), "
                   "FOREIGN KEY(userID) REFERENCES users(id)"
                   ")";

    int rc = sqlite3_exec(db, query, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Friends: " << errMsg << std::endl;
    } else {
        std::cout << "Friends created!" << std::endl;
    }
}

void DB::CreateParticipantsTable()
{
    char *errMsg = nullptr;
    char query[] = "CREATE TABLE IF NOT EXISTS participants ("
                   "userID INTEGER, "
                   "convID INTEGER, "
                   "FOREIGN KEY(convID) REFERENCES conv(convID), "
                   "FOREIGN KEY(userID) REFERENCES users(id)"
                   ")";

    int rc = sqlite3_exec(db, query, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Friends: " << errMsg << std::endl;
    } else {
        std::cout << "Friends created!" << std::endl;
    }
}

bool DB::TestUserNameLen(StringRef name)
{
    return name.length() >= 300;
}

void DB::testErr(int err)
{
    if (err == SQLITE_DONE || err == SQLITE_ROW || err == SQLITE_OK)
        return;

    throw Exception{std::to_string(err) + ": " + sqlite3_errmsg(db)};
}

bool DB::UserExists(const std::string &name)
{
    bool exists = false;
    int rc;

    char query[] = "SELECT ID FROM users WHERE name=?001";
    const char *buf = nullptr;

    sqlite3_stmt *fmt = nullptr;

    rc = sqlite3_prepare_v2(db, query, -1, &fmt, &buf);
    testErr(rc);
    rc = sqlite3_bind_text(fmt, 1, name.c_str(), name.length(), 0);
    testErr(rc);

    rc = sqlite3_step(fmt);
    testErr(rc);

    if (rc == SQLITE_ROW)
        exists = true;

    return exists;
}

bool DB::UserExists(UserID uid)
{
    bool exists = false;
    int rc;

    char query[] = "SELECT ID FROM users WHERE ID=?001";
    const char *buf = nullptr;

    sqlite3_stmt *fmt = nullptr;

    rc = sqlite3_prepare_v2(db, query, -1, &fmt, &buf);
    testErr(rc);
    rc = sqlite3_bind_int(fmt, 1, uid);
    testErr(rc);

    rc = sqlite3_step(fmt);
    testErr(rc);

    if (rc == SQLITE_ROW)
        exists = true;

    return exists;

}

UserID DB::TestCreditionals(const Credentials &creds)
{
    UserID userID = 0;
    int rc;

    char query[] = "SELECT salt, hash, id FROM users WHERE name=?001";
    const char *buf = nullptr;

    sqlite3_stmt *fmt = nullptr;

    LOGE(creds.user << " " << creds.pass << " " << userID);

    rc = sqlite3_prepare_v2(db, query, -1, &fmt, &buf);
    testErr(rc);
    rc = sqlite3_bind_text(fmt, 1, creds.user.c_str(), creds.user.length(), 0);

    testErr(rc);
    rc = sqlite3_step(fmt);
    testErr(rc);

    if (rc == SQLITE_ROW) {
        std::string saltStr = string_cast(sqlite3_column_text(fmt, 0));
        std::string passHashDB = string_cast(sqlite3_column_text(fmt, 1));

        auto salt = Hash::salt(saltStr);
        auto passHashClient = Hash::pbkdf2(creds.pass, salt);

        if(passHashClient == passHashDB)
            userID = sqlite3_column_int(fmt, 2);

    } else if (rc != SQLITE_DONE && rc != SQLITE_OK) {
        throw Exception(String{"TestCreditionals: "} + sqlite3_errstr(rc));
    }

    sqlite3_finalize(fmt);

    LOGD(creds.user << " " << creds.pass << " " << userID);

    return userID;
}

UserList DB::FindUsers(const FindUserRequest &f) {
    UserList list{};

    int rc = 0;

    auto *query = "SELECT ID, name FROM users WHERE name LIKE ?001";

    std::string in{};

    if (f.type == FindUserRequest::Type::Both ||
            f.type == FindUserRequest::Type::LeftSide)
        in += "%";

    in += f.name;

    if (f.type == FindUserRequest::Type::Both ||
            f.type == FindUserRequest::Type::LeftSide)
        in += "%";

    const char *buf = nullptr;

    sqlite3_stmt *stmt = nullptr;

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, &buf);
    testErr(rc);
    rc = sqlite3_bind_text(stmt, 1, in.c_str(), in.length(), 0);
    testErr(rc);

    do {

        rc = sqlite3_step(stmt);
        testErr(rc);

        if (rc == SQLITE_DONE)
            break;

        if (rc != SQLITE_ROW)
            throw Exception("sql err");

        User u{};
        u.ID = sqlite3_column_int(stmt, 0);
        u.name = string_cast(sqlite3_column_text(stmt, 1));

        list.push_back(u);


    } while (true);

    return list;
}

bool DB::AreFriends(UserID a, UserID b)
{
    char query[] = "SELECT accepted FROM friends WHERE (user_a=?001 AND user_b=?002) "
                   "OR (user_a=?002 AND user_b=?001)";
    int rc = 0;
    const char *buf = nullptr;
    sqlite3_stmt *stmt = nullptr;

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, &buf);
    testErr(rc);
    rc = sqlite3_bind_int(stmt, 1, a);
    testErr(rc);
    rc = sqlite3_bind_int(stmt, 2, b);
    testErr(rc);
    rc = sqlite3_step(stmt);
    testErr(rc);

    if (rc != SQLITE_ROW)
        return false;

    int accepted = sqlite3_column_int(stmt, 0);
    return accepted;
}

bool DB::RequestIsPending(UserID user, UserID f)
{
    char query[] = "SELECT accepted FROM friends WHERE user_a=?001 AND user_b=?002";
    int rc = 0;
    const char *buf = nullptr;
    sqlite3_stmt *stmt = nullptr;

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, &buf);
    testErr(rc);
    rc = sqlite3_bind_int(stmt, 1, user);
    testErr(rc);
    rc = sqlite3_bind_int(stmt, 2, f);
    testErr(rc);
    rc = sqlite3_step(stmt);
    testErr(rc);

    bool ret = rc == SQLITE_ROW;

    sqlite3_finalize(stmt);

    return ret;
}

bool DB::SendRequest(UserID user, UserID f)
{
    if (RequestIsPending(f, user))
        return AcceptFriend(f, user);

    char query[] = "INSERT INTO friends (user_a, user_b) VALUES(?, ?)";
    int rc = 0;
    const char *buf = nullptr;
    sqlite3_stmt *stmt = nullptr;

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, &buf);
    testErr(rc);
    rc = sqlite3_bind_int(stmt, 1, user);
    testErr(rc);
    rc = sqlite3_bind_int(stmt, 2, f);
    testErr(rc);
    rc = sqlite3_step(stmt);
    testErr(rc);

    return false;

}

bool DB::AcceptFriend(UserID user, UserID f)
{
    auto conv = AddFriendsConversation(user, f);

    char query[] = "UPDATE friends SET accepted='1', convID=?001 WHERE user_b=?002 AND user_a=?003";
    int rc = 0;
    const char *buf = nullptr;
    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, &buf);
    testErr(rc);
    rc = sqlite3_bind_int64(stmt, 1, conv);
    testErr(rc);
    rc = sqlite3_bind_int(stmt, 2, user);
    testErr(rc);
    rc = sqlite3_bind_int(stmt, 3, f);
    testErr(rc);
    rc = sqlite3_step(stmt);
    testErr(rc);

    LOGD("Accepted friands: " << user << " " << f << "  conv: " << conv);

    return true;
}

bool DB::DeclineFriend(UserID user, UserID f)
{
    char query[] = "DELETE FROM friends WHERE (user_b=?001 AND user_a=?002) OR (user_b=?002 AND user_a=?001)";
    int rc = 0;
    const char *buf = nullptr;
    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, &buf);
    testErr(rc);
    rc = sqlite3_bind_int(stmt, 1, user);
    testErr(rc);
    rc = sqlite3_bind_int(stmt, 2, f);
    testErr(rc);
    rc = sqlite3_step(stmt);
    testErr(rc);
    return true;
}

bool DB::RemoveFriend(UserID user, UserID f)
{
    char query[] = "DELETE FROM friends WHERE (user_b=?001 AND user_a=?002) OR (user_b=?002 AND user_a=?001)";
    int rc = 0;
    const char *buf = nullptr;
    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, &buf);
    testErr(rc);
    rc = sqlite3_bind_int(stmt, 1, user);
    testErr(rc);
    rc = sqlite3_bind_int(stmt, 2, f);
    testErr(rc);
    rc = sqlite3_step(stmt);
    testErr(rc);
    return true;
}

ConvID DB::AddFriendsConversation(UserID a, UserID b)
{
    uint64 guid = Hash::guid();

    // INSERT
    char query[] = "INSERT INTO conv (guid) VALUES (?)";

    int rc = 0;
    const char *buf = nullptr;
    sqlite3_stmt *stmt = nullptr;

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, &buf);
    testErr(rc);

    rc = sqlite3_bind_int64(stmt, 1, guid);
    testErr(rc);

    rc = sqlite3_step(stmt);
    testErr(rc);

    sqlite3_finalize(stmt);

    // GET ID
    char query2[] = "SELECT convID FROM conv WHERE guid=?001";

    rc = sqlite3_prepare_v2(db, query2, -1, &stmt, &buf);
    testErr(rc);

    rc = sqlite3_bind_int64(stmt, 1, guid);
    testErr(rc);

    rc = sqlite3_step(stmt);

    ConvID conv = 0;

    if (rc == SQLITE_ROW) {
        conv = sqlite3_column_int64(stmt, 0);
        LOGD("CONVID: " << conv);
    } else {
        testErr(rc);
        throw Exception{"Error getting ID of new conversation"};
    }

    sqlite3_finalize(stmt);

    AddParticipant(conv, a);
    AddParticipant(conv, b);

    return conv;
}

bool DB::AddParticipant(ConvID conv, UserID user)
{
    char query[] = "INSERT INTO participants (userID, convID) VALUES(?, ?)";

    const char *buf = nullptr;
    sqlite3_stmt *fmt = nullptr;
    int rc = 0;

    rc = sqlite3_prepare_v2(db, query, -1, &fmt, &buf);
    testErr(rc);

    rc = sqlite3_bind_int(fmt, 1, user);
    testErr(rc);

    rc = sqlite3_bind_int64(fmt, 2, conv);
    testErr(rc);

    rc = sqlite3_step(fmt);
    testErr(rc);

    return rc == SQLITE_DONE;
}

ConvList DB::GetUserConversation(UserID user)
{
    ConvList list{};

    char query[] = "SELECT convID FROM participants WHERE userID=?001";

    const char *buf = nullptr;
    sqlite3_stmt *stmt = nullptr;
    int rc = 0;

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, &buf);
    testErr(rc);

    rc = sqlite3_bind_int64(stmt, 1, user);
    testErr(rc);

    do {
        rc = sqlite3_step(stmt);
        testErr(rc);

        if (rc == SQLITE_DONE)
            break;

        if (rc != SQLITE_ROW)
            throw Exception("sql err");

        list.push_back(GetConversation(sqlite3_column_int64(stmt, 0)));


    } while (true);

    return list;
}

Conv DB::GetConversation(ConvID convID)
{
    Conv conv;

    char query[] = "SELECT convID, public, name  FROM conv WHERE convID=?001";

    const char *buf = nullptr;
    sqlite3_stmt *stmt = nullptr;
    int rc = 0;

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, &buf);
    testErr(rc);

    rc = sqlite3_bind_int64(stmt, 1, convID);
    testErr(rc);

    rc = sqlite3_step(stmt);
    testErr(rc);

    if (rc == SQLITE_ROW) {
        conv.ID = sqlite3_column_int64(stmt, 0);
        conv.isPublic = sqlite3_column_int(stmt, 1);
        conv.name = string_cast(sqlite3_column_text(stmt, 2));
    } else {
        testErr(rc);
        throw Exception{"Error retriving conversation"};
    }

    conv.participants = GetParticipants(convID);

    return conv;
}

ParticipantList DB::GetParticipants(ConvID convID)
{
    ParticipantList list{};

    char query[] = "SELECT userID FROM participants WHERE convID=?001";

    const char *buf = nullptr;
    sqlite3_stmt *stmt = nullptr;
    int rc = 0;

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, &buf);
    testErr(rc);

    rc = sqlite3_bind_int64(stmt, 1, convID);
    testErr(rc);

    do {

        rc = sqlite3_step(stmt);
        testErr(rc);

        if (rc == SQLITE_DONE)
            break;

        if (rc != SQLITE_ROW)
            throw Exception("sql err");

        Participant p{};
        p.ID = sqlite3_column_int(stmt, 0);
        list.push_back(p);
    } while (true);

    return list;
}

UserList DB::GetUsers(UserIDs uids)
{
    UserList list{};
    auto *query = "SELECT ID, name FROM users WHERE ID=?001";

    int rc = 0;
    const char *buf = nullptr;
    sqlite3_stmt *stmt = nullptr;

    for (auto id : uids) {

        rc = sqlite3_prepare_v2(db, query, -1, &stmt, &buf);
        testErr(rc);
        rc = sqlite3_bind_int(stmt, 1, id);
        testErr(rc);

        rc = sqlite3_step(stmt);


        if (rc == SQLITE_ROW) {
            User u{};
            u.ID = sqlite3_column_int(stmt, 0);
            u.name = string_cast(sqlite3_column_text(stmt, 1));
            list.push_back(u);
        } else {
            testErr(rc);
            throw Exception{"Error retriving user list"};
        }

        sqlite3_finalize(stmt);

    }

    return list;
}

UserList DB::GetFriendRequests(UserID userID)
{
    char query[] = "SELECT user_a FROM friends WHERE accepted=0 AND user_b=?001";

    int rc = 0;
    const char *buf = nullptr;
    sqlite3_stmt *stmt = nullptr;

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, &buf);
    testErr(rc);

    rc = sqlite3_bind_int(stmt, 1, userID);
    testErr(rc);

    UserIDs uids;
    do {
        rc = sqlite3_step(stmt);
        testErr(rc);

        if (rc == SQLITE_DONE)
            break;

        if (rc != SQLITE_ROW)
            throw Exception("sql err");

        UserID id = sqlite3_column_int(stmt, 0);

        uids.push_back(id);


    } while (true);

    return GetUsers(uids);
}

Messages DB::GetMessages(ConvID convID, uint32 offset)
{
    Messages msgs{};

    char query[] = "SELECT message, msgID, userID, time  FROM messages WHERE convID=?001 "
                   "ORDER BY time DESC, msgID DESC "
                   "LIMIT ?002 , ?003";

    int rc = 0;
    const char *buf = nullptr;
    sqlite3_stmt *stmt = nullptr;

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, &buf);
    testErr(rc);

    rc = sqlite3_bind_int64(stmt, 1, convID);
    testErr(rc);

    rc = sqlite3_bind_int(stmt, 2, offset);
    testErr(rc);

    rc = sqlite3_bind_int(stmt, 3, 100); // magick number ;)
    testErr(rc);

    do {

        rc = sqlite3_step(stmt);
        testErr(rc);

        if (rc == SQLITE_DONE)
            break;

        if (rc != SQLITE_ROW)
            throw Exception("sql err");

        Message m{};
        m.convID = convID;
        m.message = string_cast(sqlite3_column_text(stmt, 0));
        m.msgID = sqlite3_column_int(stmt, 1);
        m.userID = sqlite3_column_int(stmt, 2);
        m.time  = sqlite3_column_int(stmt, 3);

        msgs.push_back(m);


    } while (true);

    return msgs;
}

Messages DB::GetMessagesAfter(ConvID convID, MsgID msgID)
{
    Messages msgs{};

    char query[] = "SELECT message, msgID, userID, time  FROM messages WHERE convID=?001 AND msgID > ?002"
                                       "ORDER BY time DESC, msgID DESC";

    int rc = 0;
    const char *buf = nullptr;
    sqlite3_stmt *stmt = nullptr;

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, &buf);
    testErr(rc);

    rc = sqlite3_bind_int64(stmt, 1, convID);
    testErr(rc);

    rc = sqlite3_bind_int64(stmt, 2, msgID);
    testErr(rc);

    do {

        rc = sqlite3_step(stmt);
        testErr(rc);

        if (rc == SQLITE_DONE)
            break;

        if (rc != SQLITE_ROW)
            throw Exception("sql err");

        Message m{};
        m.convID = convID;
        m.message = string_cast(sqlite3_column_text(stmt, 0));
        m.msgID = sqlite3_column_int(stmt, 1);
        m.userID = sqlite3_column_int(stmt, 2);
        m.time  = sqlite3_column_int(stmt, 3);

        msgs.push_back(m);


    } while (true);

    return msgs;
}

bool DB::AddMsg(UserID userID, ConvID convID, StringRef msg)
{
    if (!IsParticipant(convID, userID))
        return false;

    char query[] = "INSERT INTO messages (convID, userID, message) VALUES(?,?,?)";

    int rc = 0;
    const char *buf = nullptr;
    sqlite3_stmt *stmt = nullptr;

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, &buf);
    testErr(rc);

    rc = sqlite3_bind_int64(stmt, 1, convID);
    testErr(rc);

    rc = sqlite3_bind_int(stmt, 2, userID);
    testErr(rc);

    rc = sqlite3_bind_text(stmt, 3, msg.c_str(), msg.length(), 0);
    testErr(rc);

    rc = sqlite3_step(stmt);
    testErr(rc);

    return rc == SQLITE_DONE;
}

bool DB::IsParticipant(ConvID convID, UserID userID)
{
    char query[] = "SELECT * FROM participants WHERE userID=?001 AND convID=?002";

    int rc = 0;
    const char *buf = nullptr;
    sqlite3_stmt *stmt = nullptr;

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, &buf);
    testErr(rc);

    rc = sqlite3_bind_int(stmt, 1, userID);
    testErr(rc);

    rc = sqlite3_bind_int64(stmt, 2, convID);
    testErr(rc);

    rc = sqlite3_step(stmt);
    testErr(rc);

    return rc == SQLITE_ROW;
}

bool DB::AddUser(const std::string &name, const std::string &pass)
{
    if (UserExists(name))
        return false;

    int rc = 0;

    char query[] = "INSERT INTO users (name, hash, salt) VALUES(?, ?, ?)";
    const char *buf = nullptr;

    sqlite3_stmt *fmt = nullptr;

    auto salt = Hash::genSalt();
    auto saltStr = Hash::toHex(salt);

    auto passHash = Hash::pbkdf2(pass, salt);

    rc = sqlite3_prepare_v2(db, query, -1, &fmt, &buf);
    testErr(rc);
    rc = sqlite3_bind_text(fmt, 1, name.c_str(), name.length(), 0);
    testErr(rc);
    rc = sqlite3_bind_text(fmt, 2, passHash.c_str(), passHash.length(), 0);
    testErr(rc);
    rc = sqlite3_bind_text(fmt, 3, saltStr.c_str(), saltStr.length(), 0);
    testErr(rc);

    rc = sqlite3_step(fmt);
    testErr(rc);

    return rc == SQLITE_DONE;
}
