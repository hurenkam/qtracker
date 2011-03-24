#include <QDebug>
#include "client.h"

Client::Client(QObject *parent)
    : QObject(parent)
    , publisher(0)
    , subscriber(0)
{
    qDebug() << "Client::Client()";
}

Q_INVOKABLE void Client::startServer()
{
    qDebug() << "Client::startServer()";
    process.start("server.exe");

    if (!publisher)
        publisher  = new QValueSpacePublisher   ("/server/command");
    if (!subscriber)
        subscriber = new  QValueSpaceSubscriber ("/server/response");
}

Q_INVOKABLE void Client::sendCommand(const QVariant& value)
{
    qDebug() << "Client::sendCommand(" << value << ")";
    if (publisher)
        publisher->setValue("cmd",value);
}

Q_INVOKABLE QVariant Client::getReply()
{
    qDebug() << "Client::getReply()";
    if (subscriber)
        return subscriber->value("ack");
    else
        return 0;
}
