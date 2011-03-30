#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QVariant>
#include <QValueSpacePublisher>
#include <QValueSpaceSubscriber>
#include <QProcess>

using namespace QtMobility;

class Client : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant reply    READ  getReply    NOTIFY replyReady)
    Q_PROPERTY(QVariant platform READ  getPlatform NOTIFY platformChanged)

signals:
    void replyReady();
    void platformChanged();

public:
    explicit Client(QObject *parent = 0);

public slots:
    Q_INVOKABLE void sendCommand(const QVariant& cmd);
    Q_INVOKABLE void startServer();

public:
    Q_INVOKABLE QVariant getReply();
    Q_INVOKABLE QVariant getPlatform();

private:
    QValueSpacePublisher*   publisher;
    QValueSpaceSubscriber*  subscriber;
    QProcess process;
};

#endif // CLIENT_H
