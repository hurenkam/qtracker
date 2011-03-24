#include <QDebug>
#include "deviceinfomodel.h"

DeviceInfoModel::DeviceInfoModel()
{
    if (!p) p = new PrivateDeviceInfoModel();
    connect(p, SIGNAL(batteryLevelChanged()), this, SLOT(onBatteryLevelChanged()));
}

void DeviceInfoModel::onBatteryLevelChanged()
{
    emit batteryLevelChanged();
}

int DeviceInfoModel::batteryLevel()
{
    return p->batteryLevel();
}

#ifdef Q_OS_SYMBIAN
int DeviceInfoModel::heapSize()
{
    TInt size;
    size = User::Heap().Size();  // Current heap size
    return size;
}

int DeviceInfoModel::heapAllocated()
{
    TInt mem;
    User::Heap().AllocSize(mem);  // Amount allocated
    return mem;
}

int DeviceInfoModel::heapLimit()
{
    TInt limit;
    limit = User::Heap().MaxLength();  // Max heap size
    return limit;
}
#endif

//============================================================================================


PrivateDeviceInfoModel *DeviceInfoModel::p = 0;

PrivateDeviceInfoModel::PrivateDeviceInfoModel(QObject *parent): QObject(parent)
{
    connect(&info, SIGNAL(batteryLevelChanged(int)), this, SLOT(onBatteryLevelChanged(int)));
}

void PrivateDeviceInfoModel::onBatteryLevelChanged(int)
{
    emit batteryLevelChanged();
}

int PrivateDeviceInfoModel::batteryLevel()
{
    return info.batteryLevel();
}
