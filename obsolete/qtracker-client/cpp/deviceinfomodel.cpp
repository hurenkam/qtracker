#include <QDebug>
#include "deviceinfomodel.h"

//#define ENABLE_DEBUG
#include "helpers.h"

DeviceInfoModel::DeviceInfoModel()
{
    ENTER("")
    if (!p) p = new PrivateDeviceInfoModel();
    connect(p, SIGNAL(batteryLevelChanged()), this, SLOT(onBatteryLevelChanged()));
    EXIT("")
}

void DeviceInfoModel::onBatteryLevelChanged()
{
    ENTER("")
    emit batteryLevelChanged();
    EXIT("")
}

int DeviceInfoModel::batteryLevel()
{
    ENTER("")
    return p->batteryLevel();
}

#ifdef Q_OS_SYMBIAN
int DeviceInfoModel::heapSize()
{
    ENTER("")
    TInt size;
    size = User::Heap().Size();  // Current heap size
    return size;
}

int DeviceInfoModel::heapAllocated()
{
    ENTER("")
    TInt mem;
    User::Heap().AllocSize(mem);  // Amount allocated
    return mem;
}

int DeviceInfoModel::heapLimit()
{
    ENTER("")
    TInt limit;
    limit = User::Heap().MaxLength();  // Max heap size
    return limit;
}
#endif

//============================================================================================


PrivateDeviceInfoModel *DeviceInfoModel::p = 0;

PrivateDeviceInfoModel::PrivateDeviceInfoModel(QObject *parent): QObject(parent)
{
    ENTER("")
    connect(&info, SIGNAL(batteryLevelChanged(int)), this, SLOT(onBatteryLevelChanged(int)));
    EXIT("")
}

void PrivateDeviceInfoModel::onBatteryLevelChanged(int)
{
    ENTER("")
    emit batteryLevelChanged();
    EXIT("")
}

int PrivateDeviceInfoModel::batteryLevel()
{
    ENTER("")
    return info.batteryLevel();
}
