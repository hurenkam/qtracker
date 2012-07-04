#ifndef TIMEDATA_H
#define TIMEDATA_H

#include <QObject>
#include <QTime>
#include <QVariant>
#include "xmlrpcserver.h"

class TimeData : public QObject
{
    Q_OBJECT
public:
    explicit TimeData(QObject *parent = 0);
    void registerApi(XmlRpcServer* srv);

signals:
    void timeChanged(QDateTime value);
    
public slots:
    void refresh();
    QVariant data();
    QVariant reset()                          { return QVariant(0); }

public:
    QDateTime elapsed();

    int       mask;
    QDateTime current;
    QDateTime starttime;
    QDateTime monitor;
};

#endif // TIMEDATA_H
