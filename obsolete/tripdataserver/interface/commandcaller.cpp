#include <QObject>
#include <QVariant>
#include <QTcpSocket>
#include <QTime>

#include "commandcaller.h"

//#define ENABLE_DEBUG
#include "helpers.h"

CommandCaller::CommandCaller(quint16 port, QObject *parent)
    : QObject(parent), serverport(port), cmdPending(0)
{
    ENTER(port)

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(onResult()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
         this, SLOT(reportError(QAbstractSocket::SocketError)));

    EXIT()
}

void CommandCaller::commandExecuted(Command* cmd)
{}

void CommandCaller::commandFailed(QAbstractSocket::SocketError error)
{}

bool CommandCaller::executeCommand(Command* cmd)
{
    if (cmdPending)
    {
        LOG("CommandCaller::executeCommand(" << cmd->cmd() << cmd->args() << "): Failed; command pending.")
        delete cmd;
        return false;
    }
    LOG( "CommandCaller::executeCommand(" << cmd->cmd() << cmd->args() << ")")
    cmdPending = cmd;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << cmdPending->cmd();
    out << cmdPending->args();

    socket->abort();
    socket->connectToHost("localhost",11120);
    socket->waitForConnected();
    socket->write(block);
    socket->flush();

    return true;
}

void CommandCaller::reportError(QAbstractSocket::SocketError error)
{
    LOG( "CommandCaller::reportError()" << error)
    commandFailed(error);
}

void CommandCaller::onResult()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);

    QVariant result;
    if (!(in.atEnd()))
        in >> result;

    LOG( "CommandCaller::onResult(): " ) // << result)

    if (cmdPending)
    {
        cmdPending->saveResult(result);
        commandExecuted(cmdPending);
    }

    cmdPending = 0;
    socket->disconnectFromHost();
}
