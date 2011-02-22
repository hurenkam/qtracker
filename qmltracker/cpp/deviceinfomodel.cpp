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
