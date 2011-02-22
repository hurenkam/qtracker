#ifndef DEVICEINFOMODEL_H
#define DEVICEINFOMODEL_H

#include <qdeclarative.h>
#include <QStringList>
#include <QUrl>
#include <QObject>
#include <QSystemDeviceInfo>

class PrivateDeviceInfoModel;
using namespace QtMobility;

QT_BEGIN_HEADER
QT_MODULE(Declarative)

class QDeclarativeContext;
class DeviceInfoModel : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(int batterylevel READ batteryLevel NOTIFY batteryLevelChanged)

signals:
    void batteryLevelChanged();

public:
    explicit DeviceInfoModel();
    int batteryLevel();

private slots:
    void onBatteryLevelChanged();

private:
    static PrivateDeviceInfoModel *p;

    void classBegin()             {}
    void componentComplete()      {}

    Q_DISABLE_COPY(DeviceInfoModel)
};

class PrivateDeviceInfoModel: public QObject
{
    Q_OBJECT

private:
    QSystemDeviceInfo info;

signals:
    void batteryLevelChanged();

private slots:
    void onBatteryLevelChanged(int);

public:
    explicit PrivateDeviceInfoModel(QObject *parent=0);
    int batteryLevel();
};

QML_DECLARE_TYPE(DeviceInfoModel)
QT_END_HEADER

#endif // DEVICEINFOMODEL_H
