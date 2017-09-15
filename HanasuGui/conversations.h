#ifndef CONVERSATIONS_H
#define CONVERSATIONS_H

#include <QAbstractItemModel>
#include <types.h>


struct ConversationItem{
    ConvID ID;
    QString name;
    bool act = false;
    bool online = true;

    ConversationItem(ConvID id, QString name);
};

class Conversations : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ConvRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        ActRole,
        OnlineRole
    };

    explicit Conversations(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    void addConversation(ConvID id, QString name);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE void activate(const int i);

    void deactiveOld();

    void clean();
private:
    std::vector<ConversationItem> convs;

public:
    QHash<int, QByteArray> roleNames() const override;
    ConvID aID = 0;

signals:
    void conversationNameChange(QString name);
};



#endif // CONVERSATIONS_H
