#ifndef TRIPSTATUS_H
#define TRIPSTATUS_H

#include <QObject>
#include <QVariant>
#include "xmlrpcserver.h"

class TripStatus : public QObject
{
    Q_OBJECT
public:
    explicit TripStatus(QObject *parent = 0);
    void registerApi(XmlRpcServer* srv);

signals:
    
public slots:
    QVariant start(const QVariant& id, const QVariant& name);
    QVariant stop();
    QVariant data();

public:
    int     id;
    QString name;
    QString state;
};

#endif // TRIPSTATUS_H
