#ifndef TRIPSTATUS_H
#define TRIPSTATUS_H

#include <QObject>
#include <QVariant>

class TripStatus : public QObject
{
    Q_OBJECT
public:
    explicit TripStatus(QObject *parent = 0);
    
signals:
    
public slots:
    QVariant start(QVariant id, QVariant name);
    QVariant stop();
    QVariant data();

public:
    int     id;
    QString name;
    QString state;
};

#endif // TRIPSTATUS_H
