#ifndef TRACKSTATUS_H
#define TRACKSTATUS_H

#include <QObject>
#include <QVariant>

class TrackStatus : public QObject
{
    Q_OBJECT
public:
    explicit TrackStatus(QObject *parent = 0);
    
signals:
    
public slots:
    
public:
    QVariant toVariant();

    int     id;
    QString name;
    QString state;
};

#endif // TRACKSTATUS_H
