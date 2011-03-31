#include <QDebug>
#include "client.h"

#define ENABLE_DEBUG
#include "helpers.h"

Client::Client(QObject *parent)
    : QObject(parent)
    , publisher(0)
    , subscriber(0)
{
    ENTER("")
    EXIT("")
}

Q_INVOKABLE void Client::startServer()
{
    ENTER("")
    //process.start("qtrackerd.exe");

    if (!publisher)
        publisher  = new QValueSpacePublisher   ("/server/command");
    if (!subscriber)
        subscriber = new  QValueSpaceSubscriber ("/server/response");
    EXIT("")
}

Q_INVOKABLE void Client::sendCommand(const QVariant& value)
{
    ENTER("")
    if (publisher)
        publisher->setValue("cmd",value);
    EXIT("")
}

Q_INVOKABLE QVariant Client::getReply()
{
    ENTER("")
    if (subscriber)
        return subscriber->value("ack");
    else
        return -1;
}

Q_INVOKABLE QVariant Client::getPlatform()
{
    ENTER("")
    int result = -1;
#if   defined(Q_OS_SYMBIAN)
    result = 0;
#elif defined(Q_WS_MAEMO_5)
    result = 1;
#elif defined(Q_WS_SIMULATOR)
    result = 2;
#endif
    return QVariant(result);
}
