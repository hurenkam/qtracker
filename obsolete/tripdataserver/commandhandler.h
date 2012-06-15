#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <QObject>
#include <QVariant>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>

#include "interface/command.h"

class CommandHandler : public QObject
{
    Q_OBJECT

public:
    CommandHandler(quint16 port, QObject* parent=0)
        : QObject(parent)
    {
        server = new QTcpServer(this);
        if (!server->listen(QHostAddress::Any,port)) {
            qDebug() << "CommandHandler::CommandHandler(): unable to start " << port << " server!";
        }
        else
        {
            connect(server, SIGNAL(newConnection()), this, SLOT(onConnection()));
            qDebug() << "CommandHandler::CommandHandler(): service " << port << " started.";
        }
    }

private slots:
    void onConnection();

protected:
    QTcpSocket* openConnection();
    void receiveAndExecuteCommand(QTcpSocket* clientConnection);
    void sendResults(QTcpSocket* clientConnection);
    virtual void handleCommand(quint16 cmd, const QVariantList& args) {}
    virtual QVariant reportResult() { return QVariant(0); }

private:
    QTcpServer *server;
};

#endif // COMMANDHANDLER_H
