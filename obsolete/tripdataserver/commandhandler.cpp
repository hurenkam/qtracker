#include "commandhandler.h"

QTcpSocket* CommandHandler::openConnection()
{
    qDebug() << "CommandHandler::openConnection()";

    QTcpSocket *clientConnection = server->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    clientConnection->waitForReadyRead();
    return clientConnection;
}

void CommandHandler::receiveAndExecuteCommand(QTcpSocket* clientConnection)
{
    qDebug() << "CommandHandler::receiveAndExecuteCommand()";

    QDataStream in(clientConnection);
    in.setVersion(QDataStream::Qt_4_0);

    if (clientConnection->bytesAvailable() >= (int)sizeof(quint16))
    {
        quint16 cmd;
        QVariantList args;
        in >> cmd;
        in >> args;
        handleCommand(cmd,args);
    }
    else
    {
        qDebug() << "CommandHandler::receiveAndExecuteCommand(): no command data" << clientConnection->bytesAvailable();
    }
}

void CommandHandler::sendResults(QTcpSocket* clientConnection)
{
    qDebug() << "CommandHandler::sendResults()";

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << reportResult();
    clientConnection->write(block);
    clientConnection->flush();
}

void CommandHandler::onConnection()
{
    qDebug() << "CommandHandler::onConnection()";

    QTcpSocket* connection = openConnection();
    receiveAndExecuteCommand(connection);
    sendResults(connection);

    //connection->disconnectFromHost();
}
