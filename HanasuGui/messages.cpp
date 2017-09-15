#include "messages.h"

#include <QFontDatabase>
#include <QFontMetrics>

#include <iostream>
#include <QRegularExpression>

MessagesList::MessagesList(QObject *parent) : QAbstractListModel(parent)
{

}

int MessagesList::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return msgs.size();
}

QVariant MessagesList::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case IdRole:
        return (unsigned long long int)msgs[index.row()].msgID;
    case OwnRole:
        return msgs[index.row()].userID == uid;
    case ContentRole:
        return msgs[index.row()].text;
    }

    return QVariant();
}

QHash<int, QByteArray> MessagesList::roleNames() const
{
    QHash<int, QByteArray> roles;
        roles[IdRole] = "id";
        roles[OwnRole] = "owner";
        roles[ContentRole] = "content";
        return roles;
}

void MessagesList::Add(MsgID mID, UserID uID, QString t)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    msgs.push_back({mID, uID, toRichText(t)});
    endInsertRows();
    newMsgs();
}

void MessagesList::clear()
{
    beginRemoveRows(QModelIndex(), 0, msgs.size());
    msgs.clear();
    endRemoveRows();
}

QString &MessagesList::toRichText(QString &str)
{
    static QRegularExpression regex("([\\x{1F600}-\\x{1F7FF}]+)");
    str.replace(regex, "<font size=5 style='font-family:Noto Color Emoji;'>\\1</font>");
    return str;
}
