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
    
public slots:
    void refresh();
    QVariant data();
    QVariant reset()                          { return QVariant(0); }

public:
    int        mask;
    QDateTime  current;
    QDateTime  elapsed;
    QDateTime  monitor;
};

#endif // TIMEDATA_H
