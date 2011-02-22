#ifndef MONITORMODEL_H
#define MONITORMODEL_H


#include <qdeclarative.h>
#include <QStringList>
#include <QUrl>
#include <QObject>
#include "datamonitor.h"
#include "clockmodel.h"

QT_BEGIN_HEADER
QT_MODULE(Declarative)

class QDeclarativeContext;
class QModelIndex;
class PrivateMonitorModel;

class MonitorModel : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(double    distance READ distance NOTIFY distanceChanged)
    Q_PROPERTY(double    bearing  READ bearing  NOTIFY bearingChanged)
    Q_PROPERTY(QDateTime time     READ time     NOTIFY timeChanged)

public:
    double    distance();
    double    bearing();
    QDateTime time();

    Q_INVOKABLE void reset() { onReset(); }

    explicit MonitorModel(QObject *parent = 0);
    ~MonitorModel() {}

signals:
    void timeChanged();
    void bearingChanged();
    void distanceChanged();

public slots:
    void onReset();

private slots:
    void onDistanceChanged() { emit distanceChanged(); }
    void onBearingChanged()  { emit bearingChanged();  }
    void onTimeChanged()     { emit timeChanged();      }

private:
    static PrivateMonitorModel *p;

    void classBegin() {}
    void componentComplete() {}

    Q_DISABLE_COPY(MonitorModel)
};

class PrivateMonitorModel: public QObject
{
    Q_OBJECT

private:
    double    _distance;
    double    _bearing;
    QDateTime _time;
    QTimer    timer;


signals:
    void distanceChanged();
    void bearingChanged();
    void timeChanged();

public slots:
    void onReset();

private slots:
    void onTimeChanged();
    void onPositionChanged();

public:
    explicit PrivateMonitorModel(QObject *parent=0);

    double    getDistance() { return _distance; }
    double    getBearing()  { return _bearing;  }
    QDateTime getTime()     { return _time;     }
};


QML_DECLARE_TYPE(MonitorModel)
QT_END_HEADER

#endif // MONITORMODEL_H
