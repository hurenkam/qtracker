#ifndef TIMEDATA_H
#define TIMEDATA_H

#include <QObject>
#include <QTime>

class TimeData : public QObject
{
    Q_OBJECT
public:
    explicit TimeData(QObject *parent = 0);

signals:
    
public slots:
    void refresh();

public:
    QVariant toVariant();

    int        mask;
    QDateTime  current;
    QDateTime  elapsed;
    QDateTime  monitor;
};

#endif // TIMEDATA_H
