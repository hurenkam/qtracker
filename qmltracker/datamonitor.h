#ifndef DATAMONITOR_H
#define DATAMONITOR_H

#include <QtCore>
#include <QtDeclarative>
#include <QSystemDeviceInfo>

using namespace QtMobility;

class DataMonitor: public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE int batteryLevel();
    Q_INVOKABLE QString number(double r, char c, int d)  { return QString::number(r,c,d);         }
    static DataMonitor& Instance();

private:
    static DataMonitor* instance;
    DataMonitor();
};

#endif // DATAMONITOR_H
