#ifndef FRIENDSMODEL_H
#define FRIENDSMODEL_H

#include <QAbstractListModel>
#include <types.h>

class FriendsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum FriendRole {
        FriendRequest = 0,
        YourRequest,
        FriendRelated
    };

    explicit FriendsModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void Add(UserID uid, QString name, int role);
    void clean();

private:
    enum ConvRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        FriendRole
    };

    struct Friend {
        UserID uid;
        QString name;
        int role;
    };

    std::vector<Friend> friends;
};

#endif // FRIENDSMODEL_H
