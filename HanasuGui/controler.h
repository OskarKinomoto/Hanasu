#ifndef CONTROLER_H
#define CONTROLER_H

#include <QObject>
#include <QQuickItem>

#include <Protocol.hpp>
#include <Serialize.hpp>

#include "conversations.h"
#include "messages.h"
#include "friendsmodel.h"

class Controler : public QQuickItem
{
    Q_OBJECT
public:
    Controler();

    Conversations* getConv();
    MessagesList* getMsgList();
    FriendsModel* getFModel();

signals:
    void loginFailed();
    void loginSuccess();
    void registerSuccess();
    void alert(QString str);

public slots:
    void init();
    void login(QString name, QString pass);
    void registration(QString name, QString pass);
    void addFriend(QString name);

    bool fetchConv();

    void showMsgs(int convID);
    void newMsgs();

    void fetchFriends();

    void sendMsg(QString msg);

    void acceptFriend(int id);
    void rejectFriend(int id);

private:
    Protocol s;
    bool m_init = false;
    ConvID cID = 0;
    MsgID last = 0;

    LoginResponse m_login;

    ConvList convList;

    Conversations c;
    MessagesList mlist;
    FriendsModel flist;

    UserID u;
};

#endif // CONTROLER_H
