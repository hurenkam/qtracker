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
    void started(int id, QString name);
    void stopped();
    //void idChanged(int id);
    //void nameChanged(QString name);
    
public slots:
    QVariant start(const QVariant& id, const QVariant& name);
    QVariant stop();
    QVariant data();
    //void setId(int id)         { this->id = id;     emit idChanged(id);     }
    //void setName(QString name) { this->name = name; emit nameChanged(name); }

public:
    int     id;
    QString name;
    QString state;
};

#endif // TRIPSTATUS_H
