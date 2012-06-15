#ifndef COMMANDCALLER_H
#define COMMANDCALLER_H

#include <QObject>
#include <QVariant>
#include <QTcpSocket>
#include <QTime>

#include "command.h"

class CommandCaller : public QObject
{
    Q_OBJECT
public:
    explicit CommandCaller(quint16 port, QObject *parent);

protected:
    virtual void commandExecuted(Command* cmd);
    virtual void commandFailed(QAbstractSocket::SocketError error);

public slots:
    bool executeCommand(Command* cmd);

private slots:
    void reportError(QAbstractSocket::SocketError error);
    void onResult();

private:
    quint16 serverport;
    QTcpSocket *socket;
    Command* cmdPending;
};

#endif // COMMANDCALLER_H
