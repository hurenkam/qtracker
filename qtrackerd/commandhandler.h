#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <QObject>
#include <QVariant>
#include <QStringList>

class CommandHandler: public QObject
{
    Q_OBJECT
public:
    explicit CommandHandler(QObject *parent = 0) : QObject(parent) {}
    ~CommandHandler() {}

public slots:
    virtual void onStart() {}
    virtual void onStop()  {}
    virtual void onReset() {}
    virtual void onCommand(QVariant cmd);
    virtual void onCommand(int method, QVariantList args) {}

protected:
    QStringList methods;
};

#endif // COMMANDHANDLER_H
