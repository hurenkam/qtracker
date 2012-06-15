#ifndef COMMANDS_H
#define COMMANDS_H

#include <QVariant>

class Command
{
public:
    Command(quint16 id, QVariantList args=QVariantList());
    quint16 cmd();
    QVariantList args();
    virtual QVariant res();
    virtual void saveResult(const QVariant& res);

protected:
    quint16 command;
    QVariantList arguments;
    QVariant result;
};

#endif // COMMANDS_H
