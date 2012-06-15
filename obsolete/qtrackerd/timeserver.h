#ifndef TIMESERVER_H
#define TIMESERVER_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <QValueSpacePublisher>
#include "commandhandler.h"

using namespace QtMobility;

class TimeServer : public CommandHandler
{
    Q_OBJECT
public:
    explicit TimeServer(QObject *parent = 0);
    ~TimeServer();

signals:
    void started();
    void stopped();
    void currentTimeChanged(const QTime& t);
    void tripTimeChanged(const QTime& t);

public slots:
    void start();
    void stop();
    void reset();
    void onTimeChanged();
    virtual void onCommand(int method, QVariantList args);

private:
    QValueSpacePublisher* p;
    QTimer* t;
    QTime _starttime;
};

#endif // TIMESERVER_H
