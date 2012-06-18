#ifndef TRACKSTATUS_H
#define TRACKSTATUS_H

#include <QObject>
#include <QVariant>
#include "xmlrpcserver.h"

class TrackStatus : public QObject
{
    Q_OBJECT
public:
    explicit TrackStatus(QObject *parent = 0);
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

#endif // TRACKSTATUS_H
