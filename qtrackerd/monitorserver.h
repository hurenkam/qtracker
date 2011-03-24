#ifndef MONITORSERVER_H
#define MONITORSERVER_H

#include <QObject>
#include <QTime>
#include <QValueSpacePublisher>

using namespace QtMobility;

class MonitorServer : public QObject
{
    Q_OBJECT
public:
    explicit MonitorServer(QObject *parent = 0);
    ~MonitorServer();

signals:
    void started();
    void stopped();

public slots:
    void start();
    void stop();
    void reset() {}
    void onPositionChanged(double,double,double);
    void onDistanceChanged(double);
    void onTimeChanged(const QTime&);
    void onSpeedChanged(double);
    void onHeadingChanged(double);

private:
    QValueSpacePublisher* p;
};

#endif // MONITORSERVER_H
