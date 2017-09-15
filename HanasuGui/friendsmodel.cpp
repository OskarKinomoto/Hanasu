#include "friendsmodel.h"

FriendsModel::FriendsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int FriendsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return friends.size();
}

QVariant FriendsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case IdRole:
        return friends[index.row()].uid;
    case NameRole:
        return friends[index.row()].name;
    case FriendRole:
        return friends[index.row()].role;
    }

    return QVariant();
}

QHash<int, QByteArray> FriendsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[FriendRole] = "role";
    return roles;

}

void FriendsModel::Add(UserID uid, QString name, int role)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    friends.push_back({uid, name, role});
    endInsertRows();
}

void FriendsModel::clean()
{
    beginRemoveRows(QModelIndex(), 0, friends.size());
    friends.clear();
    endRemoveRows();
}
