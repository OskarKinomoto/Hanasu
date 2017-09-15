#include "conversations.h"
#include <QtQml>

Conversations::Conversations(QObject *parent)
    : QAbstractListModel(parent)
{
    qmlRegisterType<Conversations>("Conversations", 1, 0, "Conversations");
}

int Conversations::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return convs.size();
}

void Conversations::addConversation(ConvID id, QString name)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    convs.push_back({id, name});
    endInsertRows();
}

QVariant Conversations::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case IdRole:
        return (unsigned long long int)convs[index.row()].ID;
    case NameRole:
        return convs[index.row()].name;
    case ActRole:
        return convs[index.row()].act;
    case OnlineRole:
        return convs[index.row()].online;
    }

    return QVariant();
}

void Conversations::activate(const int i)
{
    if (!i)
        return;

    aID = i;
    decltype(convs.size()) j = 0;
    for (;j < convs.size(); ++j)
        if (convs[j].ID == i)
            break;

    deactiveOld();

    convs[j].act = true;
    conversationNameChange(convs[j].name);
    dataChanged(index(j), index(j), {ActRole});
}

void Conversations::deactiveOld()
{
    decltype(convs.size()) j = 0;
    for (;j < convs.size(); ++j)
        if (convs[j].act)
            break;

    convs[j].act = false;
    dataChanged(index(j), index(j), {ActRole});
}

void Conversations::clean()
{
    beginRemoveRows(QModelIndex(), 0, convs.size());
    convs.clear();
    endRemoveRows();
}

QHash<int, QByteArray> Conversations::roleNames() const
{
    QHash<int, QByteArray> roles;
        roles[IdRole] = "id";
        roles[NameRole] = "name";
        roles[ActRole] = "act";
        roles[OnlineRole] = "onl";
        return roles;
}

ConversationItem::ConversationItem(ConvID id, QString name) : ID(id), name(name)
{

}
