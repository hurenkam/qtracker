#ifndef DEVICEINFOMODEL_H
#define DEVICEINFOMODEL_H

#include <qdeclarative.h>
#include <QStringList>
#include <QUrl>
#include <QObject>
#include <QSystemDeviceInfo>
#ifdef Q_OS_SYMBIAN
#include <e32std.h>
#endif

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

#ifdef Q_OS_SYMBIAN
    Q_INVOKABLE int heapSize();
    Q_INVOKABLE int heapAllocated();
    Q_INVOKABLE int heapLimit();
#else
    Q_INVOKABLE int heapSize()       { return 0; }
    Q_INVOKABLE int heapAllocated()  { return 0; }
    Q_INVOKABLE int heapLimit()      { return 0; }
#endif

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
