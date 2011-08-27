#include <QObject>
#include <QVariant>
#include <QTcpSocket>
#include <QTime>

#include "command.h"

Command::Command(quint16 id, QVariantList args) : command(id), arguments(args)
{}

quint16 Command::cmd()
{ return command; }

QVariantList Command::args()
{ return arguments; }

QVariant Command::res()
{ return result; }

void Command::saveResult(const QVariant& res)
{
    //qDebug() << "Command::saveResult(" << res << ")";
    result = res;
}

