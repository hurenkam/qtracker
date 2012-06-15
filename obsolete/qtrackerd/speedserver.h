#ifndef SPEEDSERVER_H
#define SPEEDSERVER_H

#include <QObject>
#include <QValueSpacePublisher>
#include "commandhandler.h"

using namespace QtMobility;

class SpeedServer : public CommandHandler
{
    Q_OBJECT
public:
    explicit SpeedServer(QObject *parent = 0);
    ~SpeedServer();

signals:
    void started();
    void stopped();
    void averageChanged(double);
    void minChanged(double);
    void maxChanged(double);

public slots:
    void start();
    void stop();
    void reset();
    void onSpeedChanged(double);
    virtual void onCommand(int method, QVariantList args);
    void setCount(int c);

private:
    void UpdateCurrent(double speed);
    void UpdateMinMax();
    void UpdateAverage();

private:
    QValueSpacePublisher* p;
    QList<double> _avglist;
    int    _count;

    double _current;
    double _average;
    double _max;
    double _min;
};

#endif // SPEEDSERVER_H
