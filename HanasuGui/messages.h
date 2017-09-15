#ifndef MESSAGES_H
#define MESSAGES_H

#include <QAbstractItemModel>
#include <types.h>



class MessagesList : public QAbstractListModel
{
    Q_OBJECT

public:
    enum MsgsRoles {
        IdRole = Qt::UserRole + 1,
        OwnRole,
        ContentRole,
    };

    explicit MessagesList(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void Add(MsgID mID, UserID uID, QString t);
    void clear();

    UserID uid = 0;

    Q_INVOKABLE static QString &toRichText(QString &str);

private:
    struct MessageItem {
        MsgID msgID;
        UserID userID;
        QString text;
    };
    std::vector<MessageItem> msgs;

signals:
    void newMsgs();
};

#endif // MESSAGES_H
