#include "datamonitor.h"
#include <QDir>
#include <QUrl>
#include <QString>
#include <QStringList>
#include <QDebug>

using namespace QtMobility;
DataMonitor* DataMonitor::instance = 0;

Q_INVOKABLE int
DataMonitor::batteryLevel()
{
    QSystemDeviceInfo info;
    return info.batteryLevel();
}

DataMonitor& DataMonitor::Instance() {
    if (!instance) instance = new DataMonitor();
    return *instance;
}

DataMonitor::DataMonitor() : QObject()
{
}
